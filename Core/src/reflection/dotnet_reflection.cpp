#include "reflection/dotnet_reflection.hpp"

#include <Maths/vector2i.hpp>

#include "Coral/Attribute.hpp"
#include "csharp/dotnet_assembly.hpp"
#include "csharp/dotnet_runtime.hpp"
#include "physics/component/capsule_collider.hpp"
#include "rendering/light/light.hpp"
#include "resource/audio_track.hpp"
#include "utils/logger.hpp"

using namespace XnorCore;

void DotnetReflection::PrintTypes()
{
    const Coral::ManagedAssembly* const coreAssembly = DotnetAssembly::xnorCoreAssembly->GetCoralAssembly();
    const Coral::ManagedAssembly* const gameAssembly = DotnetRuntime::GetGameAssembly()->GetCoralAssembly();

    RegisterAllTypes();

    const Coral::Type& scriptComponentType = coreAssembly->GetType(Dotnet::XnorCoreNamespace + ".ScriptComponent");

    for (const Coral::Type* type : gameAssembly->GetTypes())
    {
        if (type->IsSubclassOf(scriptComponentType))
            RegisterScriptType(type->GetFullName());
    }

    for (auto&& it : m_DotnetTypes)
    {
        Logger::LogInfo("{} ; {}", it.first, it.second.name);
    }
}

void DotnetReflection::RegisterScriptType(const std::string& typeName)
{
    DotnetTypeInfo info = {
        .displayFunc = [&, tName = typeName](ScriptComponent* const, void* const value, const char_t* fieldName) -> void { DisplayExternalType(value, fieldName, tName); },
        .serializeFunc = [&, tName = typeName](void* const value, const std::string& fieldName) -> void { SerializeExternalType(value, fieldName, tName); },
        .deserializeFunc = [&, tName = typeName](void* const value, const std::string& fieldName) -> void { DeserializeExternalType(value, fieldName, tName); },
        .name = typeName,
        .isScriptType = true
    };

    m_DotnetTypes.emplace(typeName, info);
}

void DotnetReflection::RegisterEnumType(const std::string& typeName, const std::string& assemblyName)
{
    const Coral::ManagedAssembly* const gameAssembly = DotnetRuntime::GetAssembly(assemblyName)->GetCoralAssembly();

    Coral::Type& type = gameAssembly->GetType(typeName);
    DotnetTypeInfo info = {
        .createFunc = [&] { return type.CreateInstance(); },
        .displayFunc = [&, tName = typeName](ScriptComponent* const, void* const value, const char_t* fieldName) -> void { DisplayExternalType(value, fieldName, tName); },
        .serializeFunc = [&, tName = typeName](void* const value, const std::string& fieldName) -> void { SerializeExternalType(value, fieldName, tName); },
        .deserializeFunc = [&, tName = typeName](void* const value, const std::string& fieldName) -> void { DeserializeExternalType(value, fieldName, tName); },
        .name = typeName,
        .isScriptType = false
    };

    m_DotnetTypes.emplace(typeName, info);
}

void DotnetReflection::UnregisterScriptType(const std::string& typeName)
{
    m_DotnetTypes.erase(m_DotnetTypes.find(typeName));
}

void DotnetReflection::RegisterAllTypes()
{
    RegisterBaseType<bool_t>("System.Boolean");
    RegisterBaseType<uint8_t>("System.Byte");
    RegisterBaseType<int8_t>("System.SByte");
    RegisterBaseType<int16_t>("System.Int16");
    RegisterBaseType<uint16_t>("System.UInt16");
    RegisterBaseType<int32_t>("System.Int32");
    RegisterBaseType<uint32_t>("System.UInt32");
    RegisterBaseType<int64_t>("System.Int64");
    RegisterBaseType<uint64_t>("System.UInt64");
    RegisterBaseType<char_t>("System.Char");
    RegisterBaseType<double_t>("System.Double");
    RegisterBaseType<float_t>("System.Single");

    static const std::string XnorCoreNamespace = Dotnet::XnorCoreNamespace + '.';

    RegisterBaseType<ColorHsva>(XnorCoreNamespace + "ColorHsva");
    RegisterBaseType<Color>(XnorCoreNamespace + "Colorf");

    RegisterBaseType<Vector2i>(XnorCoreNamespace + "Vector2i");
    RegisterBaseType<Vector2>(XnorCoreNamespace + "Vector2");
    RegisterBaseType<Vector3>(XnorCoreNamespace + "Vector3");
    RegisterBaseType<Vector4>(XnorCoreNamespace + "Vector4");

    RegisterCoreType<Light>(XnorCoreNamespace + "Light");
    RegisterCoreType<Entity>(XnorCoreNamespace + "Entity");
    RegisterCoreType<CapsuleCollider>(XnorCoreNamespace + "CapsuleCollider");
    
    RegisterCoreType<Pointer<Mesh>>(XnorCoreNamespace + "MeshPointer");
    RegisterCoreType<Pointer<Animation>>(XnorCoreNamespace + "AnimationPointer");
    RegisterCoreType<Pointer<Model>>(XnorCoreNamespace + "ModelPointer");
    RegisterCoreType<Pointer<AudioTrack>>(XnorCoreNamespace + "AudioTrackPointer");
    RegisterCoreType<Pointer<Texture>>(XnorCoreNamespace + "TexturePointer");
    RegisterCoreType<Pointer<Skeleton>>(XnorCoreNamespace + "SkeletonPointer");
}

void DotnetReflection::DisplayType(ScriptComponent* const script, void* obj, const char_t* const name, const std::string& typeName)
{
    if (DotnetRuntime::IsReloadingProject())
    {
        ImGui::TextColored(static_cast<Vector4>(Color::Gray()), ".NET scripts are reloading...");
        return;
    }

    auto&& it = m_DotnetTypes.find(typeName);

    if (it == m_DotnetTypes.end())
    {
        Logger::LogError("[C# Refl] Couldn't find type named {}", typeName);
        return;
    }

    it->second.displayFunc(script, obj, name);
}

void DotnetReflection::DisplayType(ScriptComponent* const script)
{
    if (DotnetRuntime::IsReloadingProject())
    {
        ImGui::TextColored(static_cast<Vector4>(Color::Gray()), ".NET scripts are reloading...");
        return;
    }
    
    Coral::ManagedObject& obj = script->GetManagedObject();

    const Coral::Type& type = obj.GetType();

    const std::string text = type.GetFullName();
    Utils::AlignImGuiCursor(ImGui::CalcTextSize(text.c_str()).x);
    ImGui::TextColored(static_cast<Vector4>(Color::PaleGreen()), "%s", text.c_str());

    ImGui::PushID(script);
    for (Coral::FieldInfo& field : type.GetFields())
    {
        auto&& attributes = field.GetAttributes();

        // Discard the field if it is non-public
        bool_t discard = field.GetAccessibility() != Coral::TypeAccessibility::Public;

        for (Coral::Attribute& attribute : attributes)
        {
            Coral::String&& name = attribute.GetType().GetFullName();
            // And if it doesn't have the 'Serialized' attribute
            if (name == Dotnet::XnorCoreNamespace + ".Serialized")
                discard = false;
                
            // Discard it anyway if it has the 'NotSerialized' attribute
            if (name == Dotnet::XnorCoreNamespace + ".NotSerialized")
                discard = true;
        }
        
        if (discard)
            continue;
        
        const std::string fieldName = field.GetName();

        if (Utils::ArrayContains(IgnoredFieldNames, fieldName))
            continue;
        
        const std::string typeName = field.GetType().GetFullName();

        void* const ptr = obj.GetFieldPointer<void>(fieldName);

        ImGui::PushID(ptr);
        DisplayType(script, ptr, fieldName.c_str(), typeName);
        ImGui::PopID();
    }
    ImGui::PopID();
}

void DotnetReflection::GetScriptTypes(List<std::string>* const list)
{
    for (DotnetTypeInfo& typeInfo : m_DotnetTypes | std::views::values)
    {
        if (typeInfo.isScriptType)
            list->Add(typeInfo.name);
    }
}

void DotnetReflection::SerializeType(void* const value, const std::string& fieldName, const std::string& typeName)
{
    auto&& it = m_DotnetTypes.find(typeName);

    if (it == m_DotnetTypes.end())
    {
        Logger::LogError("[C# Refl] Couldn't find type named {}", typeName);
        return;
    }

    it->second.serializeFunc(value, fieldName);
}

void DotnetReflection::DeserializeType(void* const value, const std::string& fieldName, const std::string& typeName)
{
    auto&& it = m_DotnetTypes.find(typeName);

    if (it == m_DotnetTypes.end())
    {
        Logger::LogError("[C# Refl] Couldn't find type named {}", typeName);
        return;
    }

    it->second.deserializeFunc(value, fieldName);
}

void DotnetReflection::SerializeExternalType(void* const value, const std::string& fieldName, const std::string& typeName)
{
    const Coral::Type& type = DotnetRuntime::GetGameAssembly()->GetCoralAssembly()->GetType(typeName);
    
    if (type.IsEnum())
    {
        const DotnetEnumInfo& info = GetEnumInfo(typeName);

        const std::vector<std::string>& enumNames = info.names;
        const std::vector<int32_t>& enumValues = info.values;

        const int32_t val = *static_cast<int32_t* const>(value);
        for (size_t i = 0; i < enumNames.size(); i++)
        {
            if (enumValues[i] == val)
            {
                Serializer::AddSimpleValue(fieldName, enumNames[i]);
                break;
            }
        }
    }
}

void DotnetReflection::DeserializeExternalType(void* const value, const std::string& fieldName, const std::string& typeName)
{
    const Coral::Type& type = DotnetRuntime::GetGameAssembly()->GetCoralAssembly()->GetType(typeName);
    
    if (type.IsEnum())
    {
        const char_t* const xmlValue = Serializer::ReadElementValue(fieldName);

        const DotnetEnumInfo& info = GetEnumInfo(typeName);

        const std::vector<std::string>& enumNames = info.names;
        const std::vector<int32_t>& enumValues = info.values;

        int32_t* const val = static_cast<int32_t* const>(value);
        for (size_t i = 0; i < enumNames.size(); i++)
        {
            if (enumNames[i] == xmlValue)
            {
                *val = enumValues[i];
                break;
            }
        }
    }
}

void DotnetReflection::DisplayExternalType(void* const value, const char_t* const fieldName, const std::string& typeName)
{
    const Coral::Type& type = DotnetRuntime::GetGameAssembly()->GetCoralAssembly()->GetType(typeName);
    
    if (type.IsEnum())
    {
        int32_t* const val = static_cast<int32_t* const>(value);

        const DotnetEnumInfo& info = GetEnumInfo(typeName);

        const std::vector<std::string>& enumNames = info.names;
        const std::vector<int32_t>& enumValues = info.values;

        if (ImGui::BeginCombo(fieldName, enumNames[std::distance(enumValues.begin(), std::ranges::find(enumValues, *val))].c_str()))
        {
            for (size_t i = 0; i < enumNames.size(); i++)
            {
                if (ImGui::Selectable(enumNames[i].c_str()))
                    *val = enumValues[i];
            }
            ImGui::EndCombo();
        }
    }
}

DotnetReflection::DotnetEnumInfo& DotnetReflection::GetEnumInfo(const std::string& name)
{
    if (m_DotnetEnums.contains(name))
        return m_DotnetEnums[name];
    
    const Coral::Type& type = DotnetRuntime::GetGameAssembly()->GetCoralAssembly()->GetType(name);

    std::vector<Coral::String> enumNames;
    type.GetEnumNames(enumNames);

    std::vector<std::string> enumNamesStr(enumNames.size());
    std::ranges::transform(enumNames, enumNamesStr.begin(), [](const Coral::String& str) -> std::string { return str; });
        
    std::vector<int32_t> enumValues;
    type.GetEnumValues(enumValues);

    m_DotnetEnums.emplace(name, DotnetEnumInfo{ enumNamesStr, enumValues, name });
    
    return m_DotnetEnums.at(name);
}

void DotnetReflection::SerializeScript(const ScriptComponent* const script)
{
    const Coral::ManagedObject& obj = script->GetManagedObject();
    const Coral::Type& type = obj.GetType();

    const auto name = type.GetFullName();
    Serializer::AddAttribute("managedType", name);
    for (Coral::FieldInfo& field : type.GetFields())
    {
        if (field.GetAccessibility() != Coral::TypeAccessibility::Public)
            continue;
        
        const std::string fieldName = field.GetName();

        if (Utils::ArrayContains(IgnoredFieldNames, fieldName))
            continue;
        
        const std::string typeName = field.GetType().GetFullName();

        void* const ptr = const_cast<Coral::ManagedObject&>(obj).GetFieldPointer<void>(fieldName);

        SerializeType(ptr, fieldName, typeName);
    }
}

void DotnetReflection::DeserializeScript(ScriptComponent* const script)
{
    Coral::ManagedObject& obj = script->GetManagedObject();
    const Coral::Type& type = obj.GetType();
    
    for (Coral::FieldInfo& field : type.GetFields())
    {
        if (field.GetAccessibility() != Coral::TypeAccessibility::Public)
            continue;
        
        const std::string fieldName = field.GetName();

        if (Utils::ArrayContains(IgnoredFieldNames, fieldName))
            continue;
        
        const std::string typeName = field.GetType().GetFullName();

        void* const ptr = obj.GetFieldPointer<void>(fieldName);

        DeserializeType(ptr, fieldName, typeName);
    }
}
