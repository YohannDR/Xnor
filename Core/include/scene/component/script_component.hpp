#pragma once

#include "core.hpp"
#include "Coral/ManagedObject.hpp"
#include "csharp/dotnet_assembly.hpp"
#include "physics/component/collider.hpp"
#include "reflection/reflection.hpp"
#include "scene/component.hpp"

/// @file script_component.hpp
/// @brief Defines the XnorCore::ScriptComponent class.

BEGIN_XNOR_CORE

/// @brief Base class for custom .NET behaviors attached to @ref Entity "Entities".
class ScriptComponent : public Component
{
#ifndef SWIG
    REFLECTABLE_IMPL(ScriptComponent)

public:
    ScriptComponent() = default;

    /// @brief Creates and initializes a new ScriptComponent from the given managed type name and assembly.
    XNOR_ENGINE static ScriptComponent* New(const std::string& managedTypeName, const DotnetAssembly* assembly);

    /// @brief Destroys this ScriptComponent in the .NET runtime.
    XNOR_ENGINE void Destroy() override;

    XNOR_ENGINE void Begin() override;

    XNOR_ENGINE void Update() override;

    /// @brief Returns the .NET ManagedObject linked to this ScriptComponent
    [[nodiscard]]
    XNOR_ENGINE Coral::ManagedObject& GetManagedObject();

    /// @brief Returns the .NET ManagedObject linked to this ScriptComponent
    [[nodiscard]]
    XNOR_ENGINE const Coral::ManagedObject& GetManagedObject() const;

protected:
    XNOR_ENGINE virtual void OnTriggerEnter(Collider* self, Collider* other, const CollisionData& data);

    XNOR_ENGINE virtual void OnTriggerStay(Collider* self, Collider* other, const CollisionData& data);

    XNOR_ENGINE virtual void OnTriggerExit(Collider* self, Collider* other);

    XNOR_ENGINE virtual void OnCollisionEnter(Collider* self, Collider* other, const CollisionData& data);

    XNOR_ENGINE virtual void OnCollisionStay(Collider* self, Collider* other, const CollisionData& data);

    XNOR_ENGINE virtual void OnCollisionExit(Collider* self, Collider* other);
    
private:
    Coral::ManagedObject m_ManagedObject;

    void InvokeCollisionEvent(const std::string& functionName, Collider* self, Collider* other, const CollisionData& data);

    void InvokeCollisionExitEvent(const std::string& functionName, Collider* self, Collider* other);

    // The DotnetRuntime class needs to have access to the m_ManagedObject field
    friend class DotnetRuntime;
#endif
};

END_XNOR_CORE

/// @private
REFL_AUTO(
    type(XnorCore::ScriptComponent, bases<XnorCore::Component>)
);
