#pragma once

#include <stack>
#include <string>

#include <RapidXML/rapidxml.hpp>
#include <RapidXML/RapidXMLSTD.hpp>

#include <unordered_map>

#include "core.hpp"
#include "scene/entity.hpp"
#include "utils/guid.hpp"

/// @file serializer.hpp
/// @brief Defines the XnorCore::Serializer static class.

BEGIN_XNOR_CORE

/// @brief A utility class that handles XML parsing, serialization and deserialization
class Serializer
{
    STATIC_CLASS(Serializer)

    /// @brief Metadata used to process a field
    /// @tparam ReflectT Reflected top level type
    /// @tparam MemberT Member type
    /// @tparam DescriptorT Field descriptor type
    template <typename ReflectT, typename MemberT, typename DescriptorT>
    struct Metadata
    {
        /// @brief Reflected top level object
        ReflectT* topLevelObj;

        /// @brief Member name
        const char_t* name;
        /// @brief Member object
        MemberT* obj;
    };

public:
    /// @brief Starts a serialization with the provided file
    /// @param filePath File name
    XNOR_ENGINE static void StartSerialization(const std::string& filePath);

    /// @brief Starts a deserialization with the provided file
    /// @param filePath File name
    XNOR_ENGINE static void StartDeserialization(const std::string& filePath);

    /// @brief Ends the serialization
    XNOR_ENGINE static void EndSerialization();
    /// @brief Ends the deserialization
    XNOR_ENGINE static void EndDeserialization();

    /// @brief Starts a root element
    /// @param elementName Name of root
    /// @param elementValue Value of root
    XNOR_ENGINE static void BeginRootElement(const std::string& elementName, const std::string& elementValue);

    /// @brief Ends the root element
    XNOR_ENGINE static void EndRootElement();

    /// @brief Starts an XML element
    /// @param elementName Name of the element
    /// @param elementValue Value of the element
    XNOR_ENGINE static void BeginXmlElement(const std::string& elementName, const std::string& elementValue);

    /// @brief Ends an XML element
    XNOR_ENGINE static void EndXmlElement();

    /// @brief Adds an attribute to the current XML element
    /// @tparam T Value type
    /// @param attributeName Attribute name
    /// @param value Attribute value
    template <typename T>
    static void AddSimpleValue(const std::string& attributeName, const T& value);

    /// @brief Serializes an object in the current document
    /// @tparam ReflectT Type
    /// @tparam IsRoot Is root object
    /// @param obj Object pointer
    template <typename ReflectT, bool_t IsRoot>
    static void Serialize(const ReflectT* obj);

    /// @brief Deserializes an object in the current document
    /// @tparam ReflectT Type
    /// @tparam IsRoot Is root object
    /// @param obj Object pointer
    template <typename ReflectT, bool_t IsRoot>
    static void Deserialize(ReflectT* obj);

    static void AddAttribute(const std::string& name, const std::string& value);

    XNOR_ENGINE static void ReadElement(const std::string& name);
    [[nodiscard]]
    XNOR_ENGINE static const char_t* ReadElementValue(const std::string& name);
    XNOR_ENGINE static void FinishReadElement();
    
private:
    XNOR_ENGINE static inline std::string m_CurrentFilePath;

    XNOR_ENGINE static inline XMLFile* m_File;
    
    XNOR_ENGINE static inline XMLDocument* m_XmlDoc = nullptr;
    
    XNOR_ENGINE static inline XMLElement* m_RootElement = nullptr;

    XNOR_ENGINE static inline std::stack<XMLElement*> m_ElementsStack;

    XNOR_ENGINE static inline std::vector<size_t> m_StaticClassesPared;
    
    XNOR_ENGINE static inline std::vector<std::pair<Guid, Entity**>> m_GuidEntityMap;

    // Serialize
    XNOR_ENGINE static void OpenFileToWrite(const std::string& filePath);

    XNOR_ENGINE static void OpenFileToRead(const std::string& filePath);

    XNOR_ENGINE static void SerializeObjectUsingFactory(void* obj, size_t hash);
    XNOR_ENGINE static void DeserializeObjectUsingFactory(void* obj, size_t hash);
    XNOR_ENGINE static void* CreateObjectUsingFactory(const std::string& name, const std::string& managedTypeName = "");
    
    // Element
    XNOR_ENGINE static void AddElementToDoc();

    template <typename ReflectT>
    static void SerializeStaticFields();

    template <typename ReflectT>
    static void DeserializeStaticFields();

    template <typename ReflectT, typename MemberT, typename DescriptorT>
    static void SerializeSimpleType(const Metadata<ReflectT, MemberT, DescriptorT>& metadata);

    template <typename ReflectT, typename MemberT, typename DescriptorT>
    static void SerializeArrayType(const Metadata<ReflectT, MemberT, DescriptorT>& metadata);

    template <typename ReflectT, typename MemberT, typename DescriptorT>
    static void SerializeListType(const Metadata<ReflectT, MemberT, DescriptorT>& metadata);

    template <typename ReflectT, typename MemberT, typename DescriptorT>
    static void SerializeEnum(const Metadata<ReflectT, MemberT, DescriptorT>& metadata);


    template <typename ReflectT, typename MemberT, typename DescriptorT>
    static void DeserializePointer(const Metadata<ReflectT, MemberT, DescriptorT>& metadata);

    template <typename ReflectT, typename MemberT, typename DescriptorT>
    static void DeserializeXnorPointer(const Metadata<ReflectT, MemberT, DescriptorT>& metadata);

    template <typename ReflectT, typename MemberT, typename DescriptorT>
    static void DeserializeEnum(const Metadata<ReflectT, MemberT, DescriptorT>& metadata);

    template <typename ReflectT, typename MemberT, typename DescriptorT>
    static void DeserializeSimpleType(const Metadata<ReflectT, MemberT, DescriptorT>& metadata);

    template <typename ReflectT, typename MemberT, typename DescriptorT>
    static void DeserializeArrayType(const Metadata<ReflectT, MemberT, DescriptorT>& metadata);

    template <typename ReflectT, typename MemberT, typename DescriptorT>
    static void DeserializeListType(const Metadata<ReflectT, MemberT, DescriptorT>& metadata);
};

END_XNOR_CORE

#include "serialization/serializer.inl"
