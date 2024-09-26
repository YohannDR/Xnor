#pragma once

#include "core.hpp"
#include "utils/list.hpp"

#include <functional>

/// @file xnor_factory.hpp
/// @brief Defines the XnorCore::XnorFactory class

BEGIN_XNOR_CORE

/// @brief Provides a small RTTI that allows to access type info via a hash code or a class name
class XnorFactory
{
    STATIC_CLASS(XnorFactory)

    struct FactoryTypeInfo
    {
        std::function<void*(const std::string&)> createFunc;
        std::function<void(void*, std::pair<void*, const char_t*>*)> displayFunc;
        std::function<void(void*)> serializeFunc;
        std::function<void(void*)> deserializeFunc;
        std::function<void(const void*, void*)> cloneFunc; 

        bool_t isConstructible;
        std::string name;
        std::vector<size_t> parentClasses;
    };

public:
    /// @brief Registers a type in the factory
    /// @tparam T Type
    template <typename T>
    static void RegisterType();

    /// @brief Creates an object via a hash code
    /// @param hash Type hash code
    /// @returns Created instance (@c nullptr if can't create)
    [[nodiscard]]
    XNOR_ENGINE static void* CreateObject(size_t hash);

    /// @brief Displays an object using the TypeRenderer via a hash code
    /// @param obj Current object
    /// @param hash Type hash code
    /// @param windowInfo Window info
    XNOR_ENGINE static void DisplayObject(void* obj, size_t hash, std::pair<void*, const char_t*>* windowInfo);

    /// @brief Serialize an object via a hash code
    /// @param obj Current object
    /// @param hash Type hash code
    XNOR_ENGINE static void SerializeObject(void* obj, size_t hash);

    /// @brief Deserialize an object via a hash code
    /// @param obj Current object
    /// @param hash Type hash code
    XNOR_ENGINE static void DeserializeObject(void* obj, size_t hash);

    /// @brief Gets the name of a class via a hash code
    /// @param hash Type hash code
    [[nodiscard]]
    XNOR_ENGINE static std::string GetTypeName(size_t hash);

    /// @brief Creates an object via a type name
    /// @param name Type name
    /// @param managedTypeName
    /// @returns Created instance (@c nullptr if can't create)
    [[nodiscard]]
    XNOR_ENGINE static void* CreateObject(const std::string& name, const std::string& managedTypeName = "");

    XNOR_ENGINE static void CloneObject(const void* src, void* dst, size_t hash);

    /// @brief Helper function to register all the XnorCore types
    XNOR_ENGINE static void RegisterAllTypes();

    /// @brief Gets all the child classes registered in the factory of a specified parent type
    /// @tparam T Parent type
    /// @param names Output names
    template <typename T>
    static void FindAllChildClasses(List<std::string>* names);

    /// @brief Prints the contents of the factory
    XNOR_ENGINE static void Print();

private:
    XNOR_ENGINE static bool_t IsChildOf(size_t typeHash, size_t parentHash);

    XNOR_ENGINE static inline std::unordered_map<size_t, FactoryTypeInfo> m_FactoryMapHash;
    XNOR_ENGINE static inline std::unordered_map<std::string, FactoryTypeInfo> m_FactoryMapName;
};

END_XNOR_CORE

#include "reflection/xnor_factory.inl"
