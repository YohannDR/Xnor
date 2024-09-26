#pragma once

#include "core.hpp"
#include "reflection/type_renderer.hpp"
#include "serialization/serializer.hpp"

#include <unordered_map>

#include "scene/component/script_component.hpp"

BEGIN_XNOR_CORE

template <typename T>
void XnorFactory::RegisterType()
{
    constexpr bool_t isConstructible = !Meta::IsAbstract<T> && Meta::IsDefaultConstructible<T>;

    const size_t hash = Utils::GetTypeHash<T>();
    if (m_FactoryMapHash.contains(hash))
        return;

    using DescriptorT = decltype(Reflection::GetTypeInfo<T>());
    
    constexpr const char_t* const name = DescriptorT::name.c_str();
    const std::string humanizedName = Utils::RemoveNamespaces(std::string(name));
    
    FactoryTypeInfo info = {
        .displayFunc = [](void* const obj, std::pair<void*, const char_t*>* const windowInfo) -> void { TypeRenderer::DisplayObject<T>(static_cast<T*>(obj), windowInfo); },
        .serializeFunc = [](void* const obj) -> void { Serializer::Serialize<T, false>(static_cast<T*>(obj)); },
        .deserializeFunc = [](void* const obj) -> void { Serializer::Deserialize<T, false>(static_cast<T*>(obj)); },
        .cloneFunc = [](const void* const src, void* const dst) -> void { Reflection::Clone(static_cast<const T*>(src), static_cast<T*>(dst)); },
        .isConstructible = isConstructible,
        .name = humanizedName
    };

    if constexpr (isConstructible)
        info.createFunc = [](const std::string&) -> void* { return new T(); };
    else
        info.createFunc = [](const std::string&) -> void* { return nullptr; };

    if constexpr (Meta::IsSame<T, ScriptComponent>)
    {
        info.createFunc = [](const std::string& managedTypeName) -> void* { return ScriptComponent::New(managedTypeName, DotnetRuntime::GetGameAssembly()); };
        info.serializeFunc = [](void* const obj) -> void { DotnetReflection::SerializeScript(static_cast<ScriptComponent*>(obj)); };
        info.deserializeFunc = [](void* const obj) -> void { DotnetReflection::DeserializeScript(static_cast<ScriptComponent*>(obj)); };
    }

    refl::util::for_each(refl::util::reflect_types(DescriptorT::declared_bases), [&]<typename ParentT>(const ParentT)
    {
        info.parentClasses.push_back(Utils::GetTypeHash<typename ParentT::type>());
    });

    m_FactoryMapHash.emplace(hash, info);
    m_FactoryMapName.emplace(humanizedName, info);
}

inline bool_t XnorFactory::IsChildOf(const size_t typeHash, const size_t parentHash)
{
    auto&& it = m_FactoryMapHash.find(typeHash);

    if (it == m_FactoryMapHash.end())
    {
        Logger::LogError("Couldn't find child type : {}", typeHash);
        return false;
    }

    const std::vector<size_t>& parents = it->second.parentClasses;
    
    auto&& parentIt = std::ranges::find(parents, parentHash);

    if (parentIt != parents.end())
        return true;

    return std::ranges::any_of(parents, [&] (const size_t parent) { return IsChildOf(parent, parentHash); });
}

template <typename T>
void XnorFactory::FindAllChildClasses(List<std::string>* names)
{
    const size_t hash = Utils::GetTypeHash<T>();

    for (auto&& it : m_FactoryMapHash)
    {
        if (IsChildOf(it.first, hash))
            names->Add(it.second.name);
    }
}

END_XNOR_CORE
