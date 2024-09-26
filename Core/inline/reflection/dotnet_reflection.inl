// ReSharper disable CppTooWideScope
// ReSharper disable CppClangTidyCertErr33C
#pragma once

#include "csharp/dotnet_runtime.hpp"
#include "reflection/filters.hpp"
#include "serialization/serializer.hpp"

BEGIN_XNOR_CORE

template <typename T>
void DotnetReflection::RegisterBaseType(const std::string& typeName)
{
    const  DotnetTypeInfo info = {
        .createFunc = []() -> Coral::ManagedObject { return {}; },
        .displayFunc = [](ScriptComponent* const script, void* const obj, const char_t* const name) -> void { DisplaySimpleType<T>(script, static_cast<T*>(obj), name); },
        .serializeFunc = [](void* const value, const std::string& fieldName) -> void { SerializeSimpleType<T>(static_cast<T*>(value), fieldName); },
        .deserializeFunc = [](void* const value, const std::string& fieldName) -> void { DeserializeSimpleType<T>(static_cast<T*>(value), fieldName); },
        .name = typeName,
        .isScriptType = false
    };

    m_DotnetTypes.emplace(typeName, info);
}

template <typename T>
void DotnetReflection::RegisterCoreType(const std::string& typeName)
{
    DotnetTypeInfo info = {
        .createFunc = []() -> Coral::ManagedObject { return {}; },
        .displayFunc = [](ScriptComponent* const script, void* const obj, const char_t* const name) -> void { DisplaySimpleType<T>(script, static_cast<T*>(obj), name); },
        .serializeFunc = [](void*, const std::string&) -> void {},
        .deserializeFunc = [](void*, const std::string&) -> void {},
        .name = typeName,
        .isScriptType = false
    };

    m_DotnetTypes.emplace(typeName, info);
}

template <typename T>
void DotnetReflection::DisplaySimpleType([[maybe_unused]] ScriptComponent* const script, T* const obj, const char_t* const name)
{
    if constexpr (Meta::IsSame<T, bool_t>)
    {
        ImGui::Checkbox(name, obj);
    }
    else if constexpr (Meta::IsAny<T, uint8_t, char_t>)
    {
        ImGui::DragScalar(name, ImGuiDataType_U8, obj);
    }
    else if constexpr (Meta::IsSame<T, int8_t>)
    {
        ImGui::DragScalar(name, ImGuiDataType_S8, obj);
    }
    else if constexpr (Meta::IsSame<T, double_t>)
    {
        ImGui::DragScalar(name, ImGuiDataType_Double, obj);
    }
    else if constexpr (Meta::IsSame<T, float_t>)
    {
        ImGui::DragScalar(name, ImGuiDataType_Float, obj);
    }
    else if constexpr (Meta::IsSame<T, int32_t>)
    {
        ImGui::DragScalar(name, ImGuiDataType_S32, obj);
    }
    else if constexpr (Meta::IsSame<T, uint32_t>)
    {
        ImGui::DragScalar(name, ImGuiDataType_U32, obj);
    }
    else if constexpr (Meta::IsSame<T, int64_t>)
    {
        ImGui::DragScalar(name, ImGuiDataType_S64, obj);
    }
    else if constexpr (Meta::IsSame<T, uint64_t>)
    {
        ImGui::DragScalar(name, ImGuiDataType_U64, obj);
    }
    else if constexpr (Meta::IsSame<T, int16_t>)
    {
        ImGui::DragScalar(name, ImGuiDataType_S16, obj);
    }
    else if constexpr (Meta::IsSame<T, uint16_t>)
    {
        ImGui::DragScalar(name, ImGuiDataType_U16, obj);
    }
    else if constexpr (Meta::IsSame<T, ColorHsva>)
    {
        Color tmp = static_cast<Color>(*obj);
        ImGui::ColorPicker4(name, &tmp.r, ImGuiColorEditFlags_DisplayHSV);
        *obj = static_cast<ColorHsva>(tmp);
    }
    else if constexpr (Meta::IsSame<T, Color>)
    {
        ImGui::ColorPicker4(name, reinterpret_cast<float_t*>(obj));
    }
    else if constexpr (Meta::IsSame<T, Vector2i>)
    {
        ImGui::DragInt2(name, obj->Raw(), .1f);
    }
    else if constexpr (Meta::IsSame<T, Vector2>)
    {
        ImGui::DragFloat2(name, obj->Raw(), .1f);
    }
    else if constexpr (Meta::IsSame<T, Vector3>)
    {
        ImGui::DragFloat3(name, obj->Raw(), .1f);
    }
    else if constexpr (Meta::IsSame<T, Vector4>)
    {
        ImGui::DragFloat4(name, obj->Raw(), .1f);
    }
    else if constexpr (Meta::IsSame<T, Entity>)
    {
        // Handle an entity pointer

        ImGui::Text("%s", name);
        ImGui::SameLine();

        Entity** e = reinterpret_cast<Entity**>(obj);
        
        // Display entity name
        if (*e != nullptr)
            ImGui::Text("%s", (*e)->name.c_str());
        else
            ImGui::Text("No entity");
    }
    else if constexpr (Meta::IsBaseOf<Component, T>)
    {
        ImGui::Text("%s", name);
        ImGui::SameLine();

        // We reinterpret the pointer to a double pointer because we point to a .NET reference type which is itself a pointer
        Component** e = reinterpret_cast<Component**>(obj);
        
        // Display entity name
        if (*e != nullptr)
            ImGui::Text("%s", (*e)->entity->name.c_str());
        else
            ImGui::Text("No entity");
    }
}

template <typename T>
void DotnetReflection::SerializeSimpleType(T* const obj, const std::string& fieldName)
{
    if constexpr (Meta::IsNativeType<T> || Meta::IsMathType<T> || Meta::IsColorType<T> || Meta::IsSame<T, std::string>)
    {
        Serializer::AddSimpleValue(fieldName, *obj);
    }
    else if constexpr (Meta::IsEnum<T>)
    {
        Serializer::AddSimpleValue<std::string>(fieldName, magic_enum::enum_name<T>(*obj).data());
    }
}

template <typename T>
void DotnetReflection::DeserializeSimpleType(T* const obj, const std::string& fieldName)
{
    const char_t* const value = Serializer::ReadElementValue(fieldName);

    if (value == nullptr)
        return;

    if constexpr (Meta::IsSame<T, bool_t>)
    {
        if (std::strcmp(value, "true") == 0)
            *obj = true;
        else
            *obj = false;
    }
    else if constexpr (Meta::IsIntegral<T>)
    {
        *obj = static_cast<T>(std::atoi(value));  // NOLINT(clang-diagnostic-implicit-int-conversion, cert-err34-c)
    }
    else if constexpr (Meta::IsFloatingPoint<T>)
    {
        *obj = std::stof(value);
    }
    else if constexpr (Meta::IsSame<T, Vector2i>)
    {
        sscanf_s(value, "%d ; %d", &obj->x, &obj->y);
    }
    else if constexpr (Meta::IsSame<T, Vector2>)
    {
        sscanf_s(value, "%f ; %f", &obj->x, &obj->y);
    }
    else if constexpr (Meta::IsSame<T, Vector3>)
    {
        sscanf_s(value, "%f ; %f ; %f", &obj->x, &obj->y, &obj->z);
    }
    else if constexpr (Meta::IsSame<T, Vector4>)
    {
        sscanf_s(value, "%f ; %f ; %f ; %f", &obj->x, &obj->y, &obj->z, &obj->w);
    }
    else if constexpr (Meta::IsSame<T, Quaternion>)
    {
        sscanf_s(value, "%f ; %f ; %f ; %f", &obj->X(), &obj->Y(), &obj->Z(), &obj->W());
    }
    else if constexpr (Meta::IsSame<T, Color>)
    {
        sscanf_s(value, "%f ; %f ; %f ; %f", &obj->r, &obj->g, &obj->b, &obj->a);
    }
    else if constexpr (Meta::IsSame<T, ColorHsva>)
    {
        sscanf_s(value, "%f ; %f ; %f ; %f", &obj->h, &obj->s, &obj->v, &obj->a);
    }
    else if constexpr (Meta::IsEnum<T>)
    {
        //DeserializeEnum<ReflectT, T, DescriptorT>(metadata);
    }
    else if constexpr (Meta::IsSame<T, std::string>)
    {
        *obj = std::string(value);
    }
    else if constexpr (Meta::IsSame<T, Guid>)
    {
        *obj = Guid::FromString(value);
    }
    else
    {
        /*ReadElement(metadata.name);
        Deserialize<T, false>(obj);
        FinishReadElement();*/
    }
}

END_XNOR_CORE
