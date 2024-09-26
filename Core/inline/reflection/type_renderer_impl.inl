// ReSharper disable CppTooWideScope

#pragma once

#include "ImGui/imgui_stdlib.h"
#include "reflection/dotnet_reflection.hpp"
#include "reflection/filters.hpp"
#include "scene/component/script_component.hpp"

BEGIN_XNOR_CORE

template <typename MemberT>
struct TypeRendererImpl<MemberT, Meta::EnableIf<Meta::IsEnum<MemberT>>>
{
    template <typename ReflectT, typename DescriptorT>
    static void Render(const TypeRenderer::Metadata<ReflectT, MemberT, DescriptorT>& metadata);
};

template <typename MemberT>
struct TypeRendererImpl<MemberT, Meta::EnableIf<Meta::IsIntegralOrFloating<MemberT>>>
{
    template <typename ReflectT, typename DescriptorT>
    static void Render(const TypeRenderer::Metadata<ReflectT, MemberT, DescriptorT>& metadata);
};

template <typename T>
struct TypeRendererImpl<Pointer<T>, Meta::EnableIf<Meta::IsBaseOf<Resource, T>>>
{
    template <typename ReflectT, typename DescriptorT>
    static void Render(const TypeRenderer::Metadata<ReflectT, Pointer<T>, DescriptorT>& metadata);  
};

template <typename T>
struct TypeRendererImpl<T*, Meta::EnableIf<Meta::IsBaseOf<Resource, T>>>
{
    template <typename ReflectT, typename DescriptorT>
    static void Render(const TypeRenderer::Metadata<ReflectT, T*, DescriptorT>& metadata);  
};

template <>
struct TypeRendererImpl<Entity*>
{
    template <typename ReflectT, typename DescriptorT>
    static void Render(const TypeRenderer::Metadata<ReflectT, Entity*, DescriptorT>& metadata);
};

template <>
struct TypeRendererImpl<Component*>
{
    template <typename ReflectT, typename DescriptorT>
    static void Render(const TypeRenderer::Metadata<ReflectT, Component*, DescriptorT>& metadata);
};

template <typename T, size_t N>
struct TypeRendererImpl<T[N]>
{
    template <typename ReflectT, typename DescriptorT>
    static void Render(const TypeRenderer::Metadata<ReflectT, T[N], DescriptorT>& metadata);
};

template <typename T>
struct TypeRendererImpl<List<T>>
{
    template <typename ReflectT, typename DescriptorT>
    static void Render(const TypeRenderer::Metadata<ReflectT, List<T>, DescriptorT>& metadata);
};

template <typename KeyT, typename T>
struct TypeRendererImpl<std::map<KeyT, T>>
{
    template <typename ReflectT, typename DescriptorT>
    static void Render(const TypeRenderer::Metadata<ReflectT, std::map<KeyT, T>, DescriptorT>& metadata);
};

template <>
struct TypeRendererImpl<List<Component*>>
{
    template <typename ReflectT, typename DescriptorT>
    static void Render(const TypeRenderer::Metadata<ReflectT, List<Component*>, DescriptorT>& metadata);
};

template <typename Ret, typename... Args>
struct TypeRendererImpl<std::function<Ret(Args...)>>
{
    template <typename ReflectT, typename DescriptorT>
    static void Render(const TypeRenderer::Metadata<ReflectT, std::function<Ret(Args...)>, DescriptorT>& metadata);
};

#define TYPE_RENDERER_IMPL(type)\
template <>\
struct TypeRendererImpl<type>\
{\
    template <typename ReflectT, typename DescriptorT>\
    static void Render(const TypeRenderer::Metadata<ReflectT, type, DescriptorT>& metadata);\
};\

TYPE_RENDERER_IMPL(bool_t)
TYPE_RENDERER_IMPL(Vector2)
TYPE_RENDERER_IMPL(Vector2i)
TYPE_RENDERER_IMPL(Vector3)
TYPE_RENDERER_IMPL(Vector4)
TYPE_RENDERER_IMPL(Quaternion)
TYPE_RENDERER_IMPL(Color)
TYPE_RENDERER_IMPL(ColorHsva)
TYPE_RENDERER_IMPL(std::string)


template <typename MemberT>
template <typename ReflectT, typename DescriptorT>
void TypeRendererImpl<MemberT, Meta::EnableIf<Meta::IsEnum<MemberT>>>::Render(const TypeRenderer::Metadata<ReflectT, MemberT, DescriptorT>& metadata)
{
    if constexpr (Reflection::HasAttribute<Reflection::EnumFlags, DescriptorT>())
        TypeRenderer::DisplayEnumFlag<ReflectT, MemberT, DescriptorT>(metadata);
    else
        TypeRenderer::DisplayEnum<ReflectT, MemberT, DescriptorT>(metadata);
}

template <typename MemberT>
template <typename ReflectT, typename DescriptorT>
void TypeRendererImpl<MemberT, Meta::EnableIf<Meta::IsIntegralOrFloating<MemberT>>>::Render(const TypeRenderer::Metadata<ReflectT, MemberT, DescriptorT>& metadata)
{
    uint32_t type;

    // Get imgui data type based on the member type
    if constexpr (Meta::IsSame<MemberT, int64_t>)
        type = ImGuiDataType_S64;
    else if constexpr (Meta::IsSame<MemberT, uint64_t>)
        type = ImGuiDataType_U64;
    else if constexpr (Meta::IsSame<MemberT, int32_t>)
        type = ImGuiDataType_S32;
    else if constexpr (Meta::IsSame<MemberT, uint32_t>)
        type = ImGuiDataType_U32;
    else if constexpr (Meta::IsSame<MemberT, int16_t>)
        type = ImGuiDataType_S16;
    else if constexpr (Meta::IsSame<MemberT, uint16_t>)
        type = ImGuiDataType_U16;
    else if constexpr (Meta::IsSame<MemberT, int8_t>)
        type = ImGuiDataType_S8;
    else if constexpr (Meta::IsSame<MemberT, uint8_t>)
        type = ImGuiDataType_U8;
    else if constexpr (Meta::IsSame<MemberT, float_t>)
        type = ImGuiDataType_Float;
    else if constexpr (Meta::IsSame<MemberT, double_t>)
        type = ImGuiDataType_Double;

    if constexpr (Meta::IsSame<MemberT, float> && Reflection::HasAttribute<Reflection::AsAngle, DescriptorT>())
    {
        ImGui::SliderAngle(metadata.name, metadata.obj);
    }
    else if constexpr (Reflection::HasAttribute<Reflection::Range<MemberT>, DescriptorT>())
    {
        constexpr Reflection::Range<MemberT> range = Reflection::GetAttribute<Reflection::Range<MemberT>, DescriptorT>();
        // Has a range attribute, display as a slider
        ImGui::SliderScalar(metadata.name, type, metadata.obj, &range.minimum, &range.maximum);
    }
    else if constexpr (Reflection::HasAttribute<Reflection::DynamicRange<ReflectT, MemberT>, DescriptorT>())
    {
        constexpr Reflection::DynamicRange<ReflectT, MemberT> range = Reflection::GetAttribute<Reflection::DynamicRange<ReflectT, MemberT>, DescriptorT>();

        // Has a dynamic range attribute, display as a slider
        if constexpr (range.minimum == nullptr)
        {
            constexpr MemberT zero = 0;
            const MemberT max = metadata.topLevelObj->*range.maximum;
            ImGui::SliderScalar(metadata.name, type, metadata.obj, &zero, &max);    
        }
        else
        {
            // Has a dynamic range attribute, display as a slider
            const MemberT min = metadata.topLevelObj->*range.minimum;
            const MemberT max = metadata.topLevelObj->*range.maximum;
            ImGui::SliderScalar(metadata.name, type, metadata.obj, &min, &max);
        }
    }
    else
    {
        // No range attribute, display as a scalar
        ImGui::DragScalar(metadata.name, type, metadata.obj, .1f);
    }
}

template <typename ReflectT, typename DescriptorT>
void TypeRendererImpl<bool_t>::Render(const TypeRenderer::Metadata<ReflectT, bool_t, DescriptorT>& metadata)
{
    ImGui::Checkbox(metadata.name, metadata.obj);
}

template <typename ReflectT, typename DescriptorT>
void TypeRendererImpl<Vector2>::Render(const TypeRenderer::Metadata<ReflectT, Vector2, DescriptorT>& metadata)
{
    if constexpr (Reflection::HasAttribute<Reflection::GridPlotting, DescriptorT>())
    {
        TypeRenderer::DisplayGridPlotting<ReflectT, Vector2, DescriptorT>(metadata);
    }
    else
    {
        ImGui::DragFloat2(metadata.name, metadata.obj->Raw(), 0.1f);
    }
}

template <typename ReflectT, typename DescriptorT>
void TypeRendererImpl<Vector2i>::Render(const TypeRenderer::Metadata<ReflectT, Vector2i, DescriptorT>& metadata)
{
    ImGui::DragInt2(metadata.name, metadata.obj->Raw(), 0.1f);
}

template <typename ReflectT, typename DescriptorT>
void TypeRendererImpl<Vector3>::Render(const TypeRenderer::Metadata<ReflectT, Vector3, DescriptorT>& metadata)
{
    if constexpr (Reflection::HasAttribute<Reflection::AsEulerAngles, DescriptorT>())
    {
        // Supposed to be euler angles, so draw as a slider angle
        ImGui::SliderAngle3(metadata.name, metadata.obj->Raw(), -360, 360, "%.0f deg", ImGuiSliderFlags_AlwaysClamp);
    }
    else
    {
        ImGui::DragFloat3(metadata.name, metadata.obj->Raw(), 0.1f);
    }
}

template <typename ReflectT, typename DescriptorT>
void TypeRendererImpl<Vector4>::Render(const TypeRenderer::Metadata<ReflectT, Vector4, DescriptorT>& metadata)
{
    ImGui::DragFloat4(metadata.name, metadata.obj->Raw(), 0.1f);
}

template <typename ReflectT, typename DescriptorT>
void TypeRendererImpl<Quaternion>::Render(const TypeRenderer::Metadata<ReflectT, Quaternion, DescriptorT>& metadata)
{
    // Convert to euler angles first
    Vector3 euler = Utils::GetQuaternionEulerAngles(*metadata.obj);
    ImGui::SliderAngle3(metadata.name, euler.Raw());

    // Then convert back to quaternion
    *metadata.obj = Quaternion::FromEuler(euler);
}

template <typename ReflectT, typename DescriptorT>
void TypeRendererImpl<Color>::Render(const TypeRenderer::Metadata<ReflectT, Color, DescriptorT>& metadata)
{
    ImGui::ColorPicker4(metadata.name, reinterpret_cast<float_t*>(metadata.obj));
}

template <typename ReflectT, typename DescriptorT>
void TypeRendererImpl<ColorHsva>::Render(const TypeRenderer::Metadata<ReflectT, ColorHsva, DescriptorT>& metadata)
{
    Color tmp = static_cast<Color>(*metadata.obj);
    ImGui::ColorPicker4(metadata.name, &tmp.r, ImGuiColorEditFlags_DisplayHSV);
    *metadata.obj = static_cast<ColorHsva>(tmp);
}

template <typename ReflectT, typename DescriptorT>
void TypeRendererImpl<std::string>::Render(const TypeRenderer::Metadata<ReflectT, std::string, DescriptorT>& metadata)
{
    ImGui::InputText(metadata.name, metadata.obj);
}

template <typename T>
template <typename ReflectT, typename DescriptorT>
void TypeRendererImpl<Pointer<T>, Meta::EnableIf<Meta::IsBaseOf<Resource, T>>>::Render(const TypeRenderer::Metadata<ReflectT, Pointer<T>, DescriptorT>& metadata)
{
    ImGui::PushID(metadata.obj);
    
    ImGui::Text("%s", metadata.name);

    ImGui::SameLine();

    // Display resource name
    if (*metadata.obj != nullptr)
    {
        const std::string resName = metadata.obj->Get()->GetName();
        // TODO clamp resource name
        const float_t textSize = std::clamp<float_t>(ImGui::CalcTextSize(resName.c_str()).x, 0.f, 5.f);
        ImGui::SetNextItemWidth(textSize);
        ImGui::Text("%s", resName.c_str());
    }
    else
    {
        ImGui::Text("No resource");
    }

    // Pop up context to clear the resource
    if (ImGui::BeginPopupContextItem("RemoveResPopUp"))
    {
        if (ImGui::Selectable("Remove"))
            *metadata.obj = nullptr;

        ImGui::EndPopup();
    }

    // Mark as a drag drop target to receive a payload from the content browser
    if (ImGui::BeginDragDropTarget())
    {
        const ImGuiPayload* const payload = ImGui::AcceptDragDropPayload("ContentBrowserFile");

        if (payload)
        {
            // Get the received resource
            Pointer<Resource> dragged = *static_cast<Pointer<Resource>*>(payload->Data);

            // Get the raw pointer for type checks
            const Resource* raw = static_cast<Resource*>(dragged);

            // Check that the dropped resource is of the correct type
            if (Utils::GetTypeHash<Resource>(raw) == Utils::GetTypeHash<T>())
            {
                // Set resource
                *reinterpret_cast<decltype(dragged)*>(metadata.obj) = dragged;
            }
        }
            
        ImGui::EndDragDropTarget();
    }
    
    ImGui::SameLine();

    if (ImGui::Button("+"))
    {
        // Set current object as the filter target
        Filters::BeginResourceFilter();
    }

    if constexpr (!Reflection::HasAttribute<Reflection::DontExpand, DescriptorT>())
    {
        if (metadata.obj->IsValid())
        {
            const TypeRenderer::Metadata<ReflectT, T, DescriptorT> metadataPtr = {
                .topLevelObj = metadata.topLevelObj,
                .name = metadata.name,
                .obj = metadata.obj->Get(),
                .windowInfo = metadata.windowInfo
            };

            TypeRenderer::DisplaySimpleType<ReflectT, T, DescriptorT>(metadataPtr);
        }
    }

    // Check if the filter should be displayed
    Filters::FilterResources<T>(metadata.obj);

    ImGui::PopID();
}

template <typename T>
template <typename ReflectT, typename DescriptorT>
void TypeRendererImpl<T*, Meta::EnableIf<Meta::IsBaseOf<Resource, T>>>::Render(const TypeRenderer::Metadata<ReflectT, T*, DescriptorT>& metadata)
{
    if (metadata.obj)
    {
        const TypeRenderer::Metadata<ReflectT, T, DescriptorT> metadataPtr = {
            .topLevelObj = metadata.topLevelObj,
            .name = metadata.name,
            .obj = *metadata.obj,
            .windowInfo = metadata.windowInfo
        };

        TypeRenderer::DisplaySimpleType<ReflectT, T, DescriptorT>(metadataPtr);
    }
}

template <typename ReflectT, typename DescriptorT>
void TypeRendererImpl<Entity*>::Render(const TypeRenderer::Metadata<ReflectT, Entity*, DescriptorT>& metadata)
{
    // Handle an entity pointer

    ImGui::Text("%s", metadata.name);
    ImGui::SameLine();

    // Display entity name
    if (*metadata.obj != nullptr)
        ImGui::Text("%s", (*metadata.obj)->name.c_str());
    else
        ImGui::Text("No entity");

    // Pop up context to clear the entity
    if (ImGui::BeginPopupContextItem("RemoveEntityPopUp"))
    {
        if (ImGui::Selectable("Remove"))
            *metadata.obj = nullptr;

        ImGui::EndPopup();
    }

    // Mark as a drag drop target to receive a payload from the hierarchy window
    if (ImGui::BeginDragDropTarget())
    {
        const ImGuiPayload* const payload = ImGui::AcceptDragDropPayload("HierarchyEntity");
        
        if (payload)
        {
            // Get the received entity
            Entity* const dragged = *static_cast<Entity**>(payload->Data);

            // Set value
            *metadata.obj = dragged;
        }
            
        ImGui::EndDragDropTarget();
    }

    ImGui::SameLine();

    if (ImGui::Button("+"))
    {
        // Set current object as the filter target
        Filters::BeginEntityFilter();
    }

    // Check if the filter should be displayed
    Filters::FilterEntity(metadata.obj);
}

template <typename ReflectT, typename DescriptorT>
void TypeRendererImpl<Component*>::Render(const TypeRenderer::Metadata<ReflectT, Component*, DescriptorT>& metadata)
{
    // TODO make generic for abstract pointers
    // Handle a component pointer
    
    const size_t hash = Utils::GetTypeHash<Component>(*metadata.obj);

    if (dynamic_cast<ScriptComponent*>(*metadata.obj) == nullptr)
    {
        if (ImGui::CollapsingHeader(metadata.name))
        {
            TypeRenderer::DisplayObjectUsingFactory(*metadata.obj, hash, metadata.windowInfo);
        }
    }
    else
    {
        if (ImGui::CollapsingHeader(metadata.name))
        {
            DotnetReflection::DisplayType(reinterpret_cast<ScriptComponent*>(*metadata.obj));
        }
    }
}

template <typename T, size_t N>
template <typename ReflectT, typename DescriptorT>
void TypeRendererImpl<T[N]>::Render(const TypeRenderer::Metadata<ReflectT, T[N], DescriptorT>& metadata)
{
    if (ImGui::CollapsingHeader(metadata.name))
    {
        // Construct common metadata for the array type, this allows a range attribute to be used
        TypeRenderer::Metadata<ReflectT, T, DescriptorT> metadataArray = {
            .topLevelObj = metadata.topLevelObj,
            .name = "",
            .windowInfo = metadata.windowInfo
        };
        
        for (size_t i = 0; i < N; i++)
        {
            ImGui::PushID(&i + i);
            
            const std::string index = std::to_string(i);

            // Feed the element specific data in the metadata
            metadataArray.name = index.c_str();
            metadataArray.obj = &(*metadata.obj)[i];

            // Display as a simple type
            TypeRenderer::DisplaySimpleType<ReflectT, T, DescriptorT>(metadataArray);

            ImGui::PopID();
        }
    }
}

template <typename T>
template <typename ReflectT, typename DescriptorT>
void TypeRendererImpl<List<T>>::Render(const TypeRenderer::Metadata<ReflectT, List<T>, DescriptorT>& metadata)
{
    if (ImGui::CollapsingHeader(metadata.name))
    {
        if (ImGui::Selectable("Add"))
        {
            // Simply add an element to the list
            metadata.obj->Add();
        }

        // Construct common metadata for the array type, this allows a range attribute to be used
        TypeRenderer::Metadata<ReflectT, T, DescriptorT> metadataArray = {
            .topLevelObj = metadata.topLevelObj,
            .name = "",
            .windowInfo = metadata.windowInfo
        };

        size_t listSize = metadata.obj->GetSize();

        for (size_t i = 0; i < listSize; i++)
        {
            ImGui::PushID(&i + i);

            // Button to delete the current element
            if (ImGui::Button("-"))
            {
                metadata.obj->RemoveAt(i);
                --listSize;

                // Check for special cases :
                // - The element we deleted was the only one in the list, so the list is now empty
                // - The element we deleted was the last one in the list, so we don't have anything else to process
                if (listSize == 0 || i == listSize)
                {
                    ImGui::PopID();
                    break;
                }
            }
                
            ImGui::SameLine();

            // Handle inserting elements
            if (ImGui::Button("+"))
            {
                // Insert a new element at the given position
                metadata.obj->Insert(i);
                ++listSize;
            }
                
            ImGui::SameLine();

            const std::string index = std::to_string(i);

            // Feed the element specific data in the metadata
            metadataArray.name = index.c_str();
            metadataArray.obj = &(*metadata.obj)[i];

            // Display as a simple type
            TypeRenderer::DisplaySimpleType<ReflectT, T, DescriptorT>(metadataArray);

            ImGui::PopID();
        }
    }
}
template <typename ReflectT, typename DescriptorT>
void TypeRendererImpl<List<Component*>>::Render(const TypeRenderer::Metadata<ReflectT, List<Component*>, DescriptorT>& metadata)
{
    if (ImGui::CollapsingHeader(metadata.name))
    {
        if (ImGui::Selectable("Add"))
        {
            // Start component filter
            Filters::BeginComponentFilter();
        }

        // Construct common metadata for the array type, this allows a range attribute to be used
        TypeRenderer::Metadata<ReflectT, Component*, DescriptorT> metadataArray = {
            .topLevelObj = metadata.topLevelObj,
            .name = "",
            .windowInfo = metadata.windowInfo
        };

        size_t listSize = metadata.obj->GetSize();

        for (size_t i = 0; i < listSize; i++)
        {
            ImGui::PushID(&i + i);

            // Button to delete the current element
            if (ImGui::Button("-"))
            {
                (*metadata.obj)[i]->Destroy();
                --listSize;

                // Check for special cases :
                // - The element we deleted was the only one in the list, so the list is now empty
                // - The element we deleted was the last one in the list, so we don't have anything else to process
                if (listSize == 0 || i == listSize)
                {
                    ImGui::PopID();
                    break;
                }
            }
                
            ImGui::SameLine();

            ImGui::SameLine();

            const std::string index = std::to_string(i);

            // Feed the element specific data in the metadata
            metadataArray.name = index.c_str();
            metadataArray.obj = &(*metadata.obj)[i];

            // Display as a simple type
            TypeRenderer::DisplaySimpleType<ReflectT, Component*, DescriptorT>(metadataArray);

            ImGui::PopID();
        }

        // Process filter
        Component* const c = Filters::FilterComponent(metadata.obj);
        if (c != nullptr)
        {
            c->entity = metadata.topLevelObj;
        }
    }
}

template <typename KeyT, typename T>
template <typename ReflectT, typename DescriptorT>
void TypeRendererImpl<std::map<KeyT, T>>::Render(const TypeRenderer::Metadata<ReflectT, std::map<KeyT, T>, DescriptorT>& metadata)
{
    ImGui::CollapsingHeader(metadata.name);

    for (std::pair<KeyT, T> it : *metadata.obj)
    {
        TypeRenderer::Metadata<ReflectT, KeyT, DescriptorT> metadataKey =
        {
            .topLevelObj = metadata.topLevelObj,
            .name = metadata.name,
            .obj = &it.first,
            .windowInfo = metadata.windowInfo
        };
    
        TypeRenderer::Metadata<ReflectT, T, DescriptorT> metadataMap =
        {
            .topLevelObj = metadata.topLevelObj,
            .name = metadata.name,
            .obj = &it.second,
            .windowInfo = metadata.windowInfo
        };

        TypeRenderer::DisplaySimpleType<ReflectT, KeyT, DescriptorT>(metadataKey);
        ImGui::SameLine();
        TypeRenderer::DisplaySimpleType<ReflectT, T, DescriptorT>(metadataMap);
    }
}

template <typename Ret, typename... Args>
template <typename ReflectT, typename DescriptorT>
void TypeRendererImpl<std::function<Ret(Args...)>>::Render(const TypeRenderer::Metadata<ReflectT, std::function<Ret(Args...)>, DescriptorT>& metadata)
{
    ImGui::TextDisabled("Function at %p", Utils::FunctionAddress(*metadata.obj));
}

END_XNOR_CORE
