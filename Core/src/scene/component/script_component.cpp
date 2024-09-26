#include "scene/component/script_component.hpp"

#include "Coral/GC.hpp"
#include "csharp/dotnet_assembly.hpp"
#include "csharp/dotnet_constants.hpp"
#include "reflection/xnor_factory.hpp"
#include "scene/entity.hpp"

using namespace XnorCore;

ScriptComponent* ScriptComponent::New(const std::string& managedTypeName, const DotnetAssembly* assembly)
{
    Logger::LogDebug("Creating ScriptComponent instance with managed type {} from assembly {}", managedTypeName, assembly->GetName());
    Coral::Type& type = assembly->GetCoralAssembly()->GetType(managedTypeName);
    
    if (!type)
    {
        Logger::LogWarning("ScriptComponent of managed type {} couldn't be found in assembly {}", managedTypeName, assembly->GetName());
        return nullptr;
    }
    
    Coral::ManagedObject instance = type.CreateInstance();
    ScriptComponent* script = instance.GetFieldValue<ScriptComponent*>("swigCPtr");
    script->m_ManagedObject = instance;
    return script;
}

void ScriptComponent::Destroy()
{
    Logger::LogDebug("Destroying ScriptComponent instance with managed type {}", static_cast<std::string>(m_ManagedObject.GetType().GetFullName()));

    entity->RemoveComponent(this);
    entity = nullptr; // In case the C# finalizer is called late
    m_ManagedObject.Destroy();
}

void ScriptComponent::Begin()
{
    Collider* collider = nullptr;
    if (entity->TryGetComponent(&collider))
    {
        collider->onTriggerEnter += [this](Collider* self, Collider* other, const CollisionData& data) { OnTriggerEnter(self, other, data); };
        collider->onTriggerStay += [this](Collider* self, Collider* other, const CollisionData& data) { OnTriggerStay(self, other, data); };
        collider->onTriggerExit += [this](Collider* self, Collider* other) { OnTriggerExit(self, other); };
        collider->onCollisionEnter += [this](Collider* self, Collider* other, const CollisionData& data) { OnCollisionEnter(self, other, data); };
        collider->onCollisionStay += [this](Collider* self, Collider* other, const CollisionData& data) { OnCollisionStay(self, other, data); };
        collider->onCollisionExit += [this](Collider* self, Collider* other) { OnCollisionExit(self, other); };
    }
    
    m_ManagedObject.InvokeMethod("Begin");
}

void ScriptComponent::Update()
{
    m_ManagedObject.InvokeMethod("Update");
}

Coral::ManagedObject& ScriptComponent::GetManagedObject()
{
    return m_ManagedObject;
}

const Coral::ManagedObject& ScriptComponent::GetManagedObject() const
{
    return m_ManagedObject;
}

void ScriptComponent::OnTriggerEnter(Collider* self, Collider* other, const CollisionData& data)
{
    InvokeCollisionEvent("OnTriggerEnter", FORWARD(self), FORWARD(other), FORWARD(data));
}

void ScriptComponent::OnTriggerStay(Collider* self, Collider* other, const CollisionData& data)
{
    InvokeCollisionEvent("OnTriggerStay", FORWARD(self), FORWARD(other), FORWARD(data));
}

void ScriptComponent::OnTriggerExit(Collider* self, Collider* other)
{
    InvokeCollisionExitEvent("OnTriggerExit", FORWARD(self), FORWARD(other));
}

void ScriptComponent::OnCollisionEnter(Collider* self, Collider* other, const CollisionData& data)
{
    InvokeCollisionEvent("OnCollisionEnter", FORWARD(self), FORWARD(other), FORWARD(data));
}

void ScriptComponent::OnCollisionStay(Collider* self, Collider* other, const CollisionData& data)
{
    InvokeCollisionEvent("OnCollisionStay", FORWARD(self), FORWARD(other), FORWARD(data));
}

void ScriptComponent::OnCollisionExit(Collider* self, Collider* other)
{
    InvokeCollisionExitEvent("OnCollisionExit", FORWARD(self), FORWARD(other));
}

void ScriptComponent::InvokeCollisionEvent(const std::string& functionName, Collider* self, Collider* other, const CollisionData& data)
{
    const Coral::ManagedAssembly* assembly = DotnetAssembly::xnorCoreAssembly->GetCoralAssembly();
    
    Coral::Type& colliderType = assembly->GetType(Dotnet::XnorCoreNamespace + ".Collider");
    Coral::Type& collisionDataType = assembly->GetType(Dotnet::XnorCoreNamespace + ".CollisionData");

    std::array managedObjects
    {
        colliderType.CreateInstance(FORWARD(self), false),
        colliderType.CreateInstance(FORWARD(other), false),
        collisionDataType.CreateInstance(&data, false)
    };
    
    m_ManagedObject.InvokeMethod(functionName, managedObjects[0], managedObjects[1], managedObjects[2]);

    for (auto&& object : managedObjects)
        object.Destroy();
}

void ScriptComponent::InvokeCollisionExitEvent(const std::string& functionName, Collider* self, Collider* other)
{
    const Coral::ManagedAssembly* assembly = DotnetAssembly::xnorCoreAssembly->GetCoralAssembly();
    
    Coral::Type& colliderType = assembly->GetType(Dotnet::XnorCoreNamespace + ".Collider");

    std::array managedObjects
    {
        colliderType.CreateInstance(FORWARD(self), false),
        colliderType.CreateInstance(FORWARD(other), false),
    };
    
    m_ManagedObject.InvokeMethod(functionName, managedObjects[0], managedObjects[1]);

    for (auto&& object : managedObjects)
        object.Destroy();
}
