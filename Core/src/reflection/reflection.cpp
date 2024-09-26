#include "reflection/reflection.hpp"
#include "reflection/xnor_factory.hpp"
#include "scene/component/script_component.hpp"

using namespace XnorCore;

void Reflection::CloneUsingFactory(const void* const src, void* const dst, const size_t hash)
{
    XnorFactory::CloneObject(src, dst, hash);
}

void* Reflection::CreateUsingFactory(const size_t hash, const Component* obj)
{
    if (Utils::GetTypeHash<ScriptComponent>() == hash)
    {
        const std::string managedTypeName = dynamic_cast<const ScriptComponent*>(obj)->GetManagedObject().GetType().GetFullName();
        return XnorFactory::CreateObject("ScriptComponent", managedTypeName);
    }
    else
    {
        return XnorFactory::CreateObject(hash);
    }
}
