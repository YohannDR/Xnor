#pragma once

#include <functional>
#include <unordered_map>

#include "core.hpp"
#include "Coral/ManagedObject.hpp"
#include "scene/component/script_component.hpp"
#include "utils/list.hpp"

/// @file dotnet_reflection.hpp
/// @brief Defines the XnorCore::DotnetReflection class.

BEGIN_XNOR_CORE

/// @brief Provides utility functions for dotnet types reflection
class DotnetReflection
{
    STATIC_CLASS(DotnetReflection)

    struct DotnetTypeInfo
    {
        std::function<Coral::ManagedObject()> createFunc;
        std::function<void(ScriptComponent*, void*, const char_t*)> displayFunc;
        std::function<void(void*, const std::string&)> serializeFunc;
        std::function<void(void*, const std::string&)> deserializeFunc;

        std::string name;
        bool_t isScriptType = false;
    };

    struct DotnetEnumInfo
    {
        std::vector<std::string> names;
        std::vector<int32_t> values;
        
        std::string typeName;
    };

public:
    /// @brief Prints the contents of the dotnet factory
    XNOR_ENGINE static void PrintTypes();

    /// @brief Registers a base type
    /// @tparam T Type
    /// @param typeName C# type name
    template <typename T>
    static void RegisterBaseType(const std::string& typeName);

    /// @brief Registers a core type
    /// @tparam T Type
    /// @param typeName C# type name
    template <typename T>
    static void RegisterCoreType(const std::string& typeName);

    /// @brief Registers a script type
    /// @param typeName C# type name
    XNOR_ENGINE static void RegisterScriptType(const std::string& typeName);
    
    XNOR_ENGINE static void RegisterEnumType(const std::string& typeName, const std::string& assemblyName);

    /// @brief Unregisters a script type
    /// @param typeName C# type name
    XNOR_ENGINE static void UnregisterScriptType(const std::string& typeName);

    /// @brief Displays a simple type
    /// @tparam T Type
    /// @param script Script component
    /// @param obj Object pointer
    /// @param name Field name
    template <typename T>
    static void DisplaySimpleType(ScriptComponent* script, T* obj, const char_t* name);
    
    /// @brief Helper function to register all the base types
    XNOR_ENGINE static void RegisterAllTypes();

    /// @brief Displays a type using the internal dotnet factory
    /// @param script Script component
    /// @param obj Object pointer
    /// @param name Field name
    /// @param typeName C# type name
    XNOR_ENGINE static void DisplayType(ScriptComponent* script, void* obj, const char_t* name, const std::string& typeName);

    /// @brief Displays a script component type
    /// @param script Script component
    XNOR_ENGINE static void DisplayType(ScriptComponent* script);

    XNOR_ENGINE static void GetScriptTypes(List<std::string>* list);
    
    XNOR_ENGINE static void SerializeScript(const ScriptComponent* script);

    XNOR_ENGINE static void DeserializeScript(ScriptComponent* script);

private:
    static inline const std::array<std::string, 2> IgnoredFieldNames
    {
        "swigCPtr",
        "swigCMemOwn"
    };
    
    static inline std::unordered_map<std::string, DotnetTypeInfo> m_DotnetTypes;
    
    static inline std::unordered_map<std::string, DotnetEnumInfo> m_DotnetEnums;

    XNOR_ENGINE static void SerializeType(void* value, const std::string& fieldName, const std::string& typeName);

    XNOR_ENGINE static void DeserializeType(void* value, const std::string& fieldName, const std::string& typeName);

    XNOR_ENGINE static void SerializeExternalType(void* value, const std::string& fieldName, const std::string& typeName);

    XNOR_ENGINE static void DeserializeExternalType(void* value, const std::string& fieldName, const std::string& typeName);
    
    XNOR_ENGINE static void DisplayExternalType(void* value, const char_t* fieldName, const std::string& typeName);

    template <typename T>
    static void SerializeSimpleType(T* obj, const std::string& fieldName);

    template <typename T>
    static void DeserializeSimpleType(T* obj, const std::string& fieldName);

    static DotnetEnumInfo& GetEnumInfo(const std::string& name);
};

END_XNOR_CORE

#include "reflection/dotnet_reflection.inl"
