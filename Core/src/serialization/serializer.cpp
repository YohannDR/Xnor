#include "serialization/serializer.hpp"

#include "reflection/xnor_factory.hpp"
#include "utils/logger.hpp"

using namespace XnorCore;

void Serializer::OpenFileToWrite(const std::string& filePath)
{
    std::string error;
    m_CurrentFilePath = filePath;
    m_XmlDoc = CreateXML(1, "utf-16", error);
}

void Serializer::OpenFileToRead(const std::string& filePath)
{
    std::string error;
    m_CurrentFilePath = filePath;
    m_File = OpenXMLFile(filePath, error);

    if (!m_File)
    {
        Logger::LogError("Failed to open an XML file for reading : {}", filePath);
        return;
    }

    m_XmlDoc = CreateXMLFromFile(m_File, error);
}

void Serializer::SerializeObjectUsingFactory(void* const obj, const size_t hash)
{
    AddAttribute("typeName", XnorFactory::GetTypeName(hash));
    XnorFactory::SerializeObject(obj, hash);
}

void Serializer::DeserializeObjectUsingFactory(void* const obj, const size_t hash)
{
    XnorFactory::DeserializeObject(obj, hash);
}

void* Serializer::CreateObjectUsingFactory(const std::string& name, const std::string& managedTypeName)
{
    return XnorFactory::CreateObject(name, managedTypeName);
}

void Serializer::StartSerialization(const std::string& filePath)
{
    OpenFileToWrite(filePath);
    m_StaticClassesPared.clear();
}

void Serializer::StartDeserialization(const std::string& filePath)
{
    OpenFileToRead(filePath);
    m_GuidEntityMap.clear();
}

void Serializer::EndSerialization()
{
    if (!SaveXML(*m_XmlDoc, m_CurrentFilePath))
    {
        Logger::LogError("Failed to write Serialization");
    }

    DisposeXMLObject(m_XmlDoc);
    m_CurrentFilePath = "";
    m_StaticClassesPared.clear();
}

void Serializer::EndDeserialization()
{
    DisposeXMLObject(m_XmlDoc);
    DisposeXMLFile(m_File);
    Logger::Synchronize();
    m_CurrentFilePath = "";
    
    m_GuidEntityMap.clear();
}

void Serializer::BeginRootElement(const std::string& elementName, const std::string& elementValue)
{
    std::string error;
    
    m_RootElement = CreateElement(m_XmlDoc, elementName, elementValue, error);
    if (m_RootElement == nullptr)
    {
        Logger::LogError(error);
        DisposeXMLObject(m_XmlDoc);
    }

    AddElementToDoc();
}

void Serializer::EndRootElement()
{
    if (!m_ElementsStack.empty())
    {
        throw std::runtime_error("Stack is not empty at EndRootElement");
    }
}

void Serializer::BeginXmlElement(const std::string& elementName, const std::string& elementValue)
{
    std::string error;
    
    XMLElement* element = CreateElement(m_XmlDoc, elementName, elementValue, error);
    if (element == nullptr)
    {
        Logger::LogError(error);
        throw std::runtime_error("Failed to create element");
    }

    m_ElementsStack.push(element);
}

void Serializer::EndXmlElement()
{
    std::string error;

    if (m_ElementsStack.size() == 1)
    {
        if (!AddElementToElement(m_RootElement, m_ElementsStack.top(), error))
        {
            Logger::LogError(error);    
        }
        m_ElementsStack.pop();
    }
    else
    {
        XMLElement* const child = m_ElementsStack.top();
        m_ElementsStack.pop();
        XMLElement* const parent = m_ElementsStack.top();
        
        if (!AddElementToElement(parent, child, error))
        {
            Logger::LogError(error);    
        }
    }
}

void Serializer::AddAttribute(const std::string& name, const std::string& value)
{
    std::string error;
    
    XMLAttributte* const attribute = CreateAttribute(m_XmlDoc, name, value, error);
    AddAttributeToElement(m_ElementsStack.top(), attribute, error);
}

void Serializer::AddElementToDoc()
{
    std::string error;
    
    if (!::AddElementToDocument(m_XmlDoc, m_RootElement, error))
    {
        Logger::LogError(error);
        DisposeXMLObject(m_XmlDoc);
    }
}

void Serializer::ReadElement(const std::string& name)
{
    std::string error;
    XMLElement* elem;

    if (m_ElementsStack.empty())
    {
        elem = FirstOrDefaultElement(m_XmlDoc, name, error);
    }
    else
    {
        elem = FirstOrDefaultElement(m_ElementsStack.top(), name, error);
    }

    if (!elem)
    {
        Logger::LogError("Failed to read an element : {} : {}", name, error);
        return;
    }

    m_ElementsStack.push(elem);
}

const char_t* Serializer::ReadElementValue(const std::string& name)
{
    const XMLElement* const elem = m_ElementsStack.top();
    const XMLElement* const vElem = elem->first_node(name.c_str());

    if (!vElem)
    {
        // Logger::LogDebug("Couldn't find element named : {}", name);
        return nullptr;
    }

    return vElem->value();
}

void Serializer::FinishReadElement()
{
    m_ElementsStack.pop();
}
