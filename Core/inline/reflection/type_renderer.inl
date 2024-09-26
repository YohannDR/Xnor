// ReSharper disable CppTooWideScope
#pragma once

#include "ImGui/imgui.h"
#include "input/keyboard_input.hpp"
#include "magic_enum/magic_enum_all.hpp"
#include "utils/timeline.hpp"
#include "utils/utils.hpp"

BEGIN_XNOR_CORE

template <typename MemberT, typename T>
template <typename ReflectT, typename DescriptorT>
void TypeRendererImpl<MemberT, T>::Render(const TypeRenderer::Metadata<ReflectT, MemberT, DescriptorT>& metadata)
{
    if constexpr (Reflection::IsReflected<MemberT>)
        TypeRenderer::DisplayObject<MemberT>(metadata.obj, metadata.windowInfo);
    else
        Logger::LogError("Type doesn't have a TypeRenderImpl : {} of type {}", metadata.name, typeid(MemberT).name());
}

template <typename ReflectT, typename MemberT, typename DescriptorT>
void TypeRenderer::DisplayGridPlotting(const Metadata<ReflectT, MemberT, DescriptorT>& metadata)
{
    const Reflection::GridPlotting& plotting = Reflection::GetAttribute<Reflection::GridPlotting, DescriptorT>();
    
    ImGui::Text("%s", metadata.name);
    ImDrawList* const drawList = ImGui::GetWindowDrawList();

    // TODO handle resize
    constexpr Vector2 size = Vector2(100, 100);
    ImGui::InvisibleButton("##canvas", size);

    const ImVec2 p0 = ImGui::GetItemRectMin();
    const ImVec2 p1 = ImGui::GetItemRectMax();

    const Vector2 plottingRange = Vector2(plotting.minimum, plotting.maximum);
    constexpr Vector2 uniformRange = Vector2(0.f, 1.f);

    // Handle clicking
    if (ImGui::IsItemActive() && ImGui::IsMouseDown(ImGuiMouseButton_Left))
    {
        const ImGuiIO& io = ImGui::GetIO();

        // Compute new value, ranged between 0 and 1
        const Vector2 newValue = (io.MousePos - p0) / size;

        // Remap the value from [0; 1] to [min; max]
        metadata.obj->x = Utils::RemapValue(newValue.x, uniformRange, plottingRange);
        metadata.obj->y = Utils::RemapValue(newValue.y, uniformRange, plottingRange);

        // Clamp the value between min and max
        metadata.obj->x = std::clamp<float_t>(metadata.obj->x, plotting.minimum, plotting.maximum);
        metadata.obj->y = std::clamp<float_t>(metadata.obj->y, plotting.minimum, plotting.maximum);
    }

    // Create rectangle
    ImGui::PushClipRect(p0, p1, true);
    drawList->AddRectFilled(p0, p1, IM_COL32(90, 90, 120, 255));

    // Remap from [min; max] to [0, 1]
    const Vector2 clamped = Vector2(
        Utils::RemapValue(metadata.obj->x, plottingRange, uniformRange),
        Utils::RemapValue(metadata.obj->y, plottingRange, uniformRange)
    );

    // Compute cursor position
    const Vector2 position = p0 + clamped * size;

    drawList->AddCircle(position, 5, IM_COL32_WHITE);
    ImGui::PopClipRect();

    // Draw slider float version
    ImGui::SameLine();
    ImGui::VSliderFloat("##v2y", ImVec2(18, 100), &metadata.obj->y, plotting.minimum, plotting.maximum, "%.3f", ImGuiSliderFlags_AlwaysClamp);
    ImGui::SliderFloat("##v2x", &metadata.obj->x, plotting.minimum, plotting.maximum, "%.3f", ImGuiSliderFlags_AlwaysClamp);
}

template <typename ReflectT, typename MemberT, typename DescriptorT>
void TypeRenderer::DisplayEnum(const Metadata<ReflectT, MemberT, DescriptorT>& metadata)
{
    // Get an array of the enum names
    constexpr auto enumNames = magic_enum::enum_names<MemberT>();
    // Get the type, we can't know before compile time the type because the size of the array is determined when the function is processed
    using NamesArrayT = decltype(enumNames);

    // Getter for imgui
    constexpr auto getter = [](void* const userData, const int32_t idx) -> const char_t*
    {
        const auto ptr = static_cast<NamesArrayT*>(userData);
        return ptr->at(idx).data();
    };

    void* const names = reinterpret_cast<void*>(const_cast<Meta::RemoveConstSpecifier<NamesArrayT>*>(&enumNames));
    ImGui::Combo(metadata.name, reinterpret_cast<int32_t*>(metadata.obj), getter, names, static_cast<int32_t>(enumNames.size()));
}

template <typename ReflectT, typename MemberT, typename DescriptorT>
void TypeRenderer::DisplayEnumFlag(const Metadata<ReflectT, MemberT, DescriptorT>& metadata)
{
    // Get an array of the enum names
    constexpr auto enumNames = magic_enum::enum_names<MemberT>();
    constexpr size_t size = enumNames.size();

    // Compute preview value, it should display as all the selected values, comma separated
    // e.g. : Value1,Value2,Value8
    auto v = magic_enum::enum_flags_name<MemberT>(*metadata.obj, ',');
    const char_t* const previewValue = v.data();

    // Need to do a custom combo implementation because we can select multiple values
    if (ImGui::BeginCombo(metadata.name, previewValue))
    {
        for (size_t i = 0; i < size; i++)
        {
            // Get enum value
            const size_t enumValue = static_cast<size_t>(magic_enum::enum_value<MemberT>(i));
            // Get member value
            size_t value = static_cast<size_t>(*metadata.obj);
            // Check if the enum value is set
            const bool_t isSelected = (value & enumValue) != 0;

            if (ImGui::MenuItem(enumNames.at(i).data(), nullptr, isSelected))
            {
                // Special case
                if (enumValue == 0)
                {
                    // Full clear value
                    value = 0;
                }
                else
                {
                    if (isSelected)
                    {
                        // Value was previously set, remove it
                        value &= ~enumValue;
                    }
                    else
                    {
                        // Value was previously not set, add it
                        value |= enumValue;
                    }
                }

                // Update value
                *metadata.obj = static_cast<MemberT>(value);
            }
        }
        
        ImGui::EndCombo();
    }
}

template <typename ReflectT>
std::pair<void*, const char_t*> TypeRenderer::DisplayObject(ReflectT* const obj, std::pair<void*, const char_t*>* windowInfo)
{
    std::pair<void*, const char_t*> infoLocal = std::make_pair<void*, const char_t*>(nullptr, "");
    if (windowInfo == nullptr)
    {
        windowInfo = &infoLocal;
    }
    
    // Get reflected data
    using DescriptorT = decltype(Reflection::GetTypeInfo<ReflectT>());

    // Humanized type name
    const std::string typeName = Utils::RemoveNamespaces(DescriptorT::name.c_str());

    // Display the type name centered
    const float_t textSize = ImGui::CalcTextSize(typeName.c_str()).x;
    Utils::AlignImGuiCursor(textSize);
    ImGui::TextColored(static_cast<Vector4>(Color::PaleGreen()), "%s", typeName.c_str());

    if constexpr (Reflection::HasAttribute<Reflection::OpenEditorWindow, DescriptorT>())
    {
        constexpr Reflection::OpenEditorWindow window = Reflection::GetAttribute<Reflection::OpenEditorWindow, DescriptorT>();
        if (ImGui::IsItemHovered())
        {
            if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
            {
                windowInfo->first = obj;
                windowInfo->second = window.windowName;
            }
        }
    }

    DisplayFields<ReflectT, true>(obj, windowInfo);
    DisplayFields<ReflectT, false>(obj, windowInfo);

    return *windowInfo;
}

template <typename ReflectT, bool_t IsStatic>
void TypeRenderer::DisplayFields(ReflectT* const obj, std::pair<void*, const char_t*>* const windowInfo)
{
    // Get reflected data
    constexpr TypeDescriptor<ReflectT> desc = Reflection::GetTypeInfo<ReflectT>();

    bool_t hasStatic = false;

    // Loop over each reflected member
    refl::util::for_each(desc.members, [&]<typename DescriptorT>(const DescriptorT)
    {
        // Get member type
        using MemberT = Meta::RemoveConstSpecifier<typename DescriptorT::value_type>;

        // Shorthand for the templated attributes
        using NotifyChangeT = Reflection::NotifyChange<ReflectT>;
        using ModifiedCallbackT = Reflection::ModifiedCallback<ReflectT>;

        constexpr bool_t isConst = !DescriptorT::is_writable;
        constexpr bool_t hidden = Reflection::HasAttribute<Reflection::HideInInspector, DescriptorT>();
        constexpr bool_t readOnly = Reflection::HasAttribute<Reflection::ReadOnly, DescriptorT>();

        // We want to display static fields when IsStatic is true, and member fields when IsStatic is false
        // Hence the binary operations
        // Truth table :
        // IsStatic | is_static | !is_static | Result
        // 0        | 0         | 1          | 1
        // 0        | 1         | 0          | 0
        // 1        | 0         | 1          | 1
        // 1        | 1         | 0          | 0
        constexpr bool_t display = IsStatic ^ !DescriptorT::is_static;

        if constexpr (IsStatic && DescriptorT::is_static)
        {
            hasStatic = true;
        }

        constexpr bool_t notifyChange = Reflection::HasAttribute<NotifyChangeT, DescriptorT>() || Reflection::HasAttribute<ModifiedCallbackT, DescriptorT>();

        if constexpr (!hidden && display)
        {
            ImGui::BeginDisabled(isConst || readOnly);
            
            if constexpr (notifyChange)
            {
                // Need to notify if a change happened, so keep the old value
                const MemberT oldValue = DescriptorT::get(obj);

                // Display object
                DisplayObjectInternal<ReflectT, MemberT, DescriptorT>(obj, windowInfo);

                // Get the new value
                const MemberT newValue = DescriptorT::get(obj);

                if (newValue != oldValue)
                {
                    if constexpr (Reflection::HasAttribute<NotifyChangeT, DescriptorT>())
                    {
                        // Value was changed, set the pointer to true
                        constexpr NotifyChangeT notify = Reflection::GetAttribute<NotifyChangeT, DescriptorT>();
                        obj->*notify.pointer = true;
                    }
                    else if constexpr (Reflection::HasAttribute<ModifiedCallbackT, DescriptorT>())
                    {
                        constexpr ModifiedCallbackT notify = Reflection::GetAttribute<ModifiedCallbackT, DescriptorT>();
                        notify.callback(obj);
                    }
                }
            }
            else
            {
                // Don't need to notify, simply display the object
                DisplayObjectInternal<ReflectT, MemberT, DescriptorT>(obj, windowInfo);
            }

            ImGui::EndDisabled();
        }
    });

    if constexpr (IsStatic)
    {
        if (hasStatic)
        {
            ImGui::Separator();
        }
    }
}

template <typename ReflectT, typename MemberT, typename DescriptorT>
void TypeRenderer::DisplayObjectInternal(ReflectT* obj, std::pair<void*, const char_t*>* const windowInfo)
{
    // Get humanized variable name
    const std::string n = Utils::HumanizeVariableName(DescriptorT::name.c_str());
    const char_t* const name = n.c_str();

    // Construct metadata
    const Metadata<ReflectT, MemberT, DescriptorT> metadata = {
        .topLevelObj = obj,
        .name = name,
        .obj = [&]() -> MemberT*
        {
            if constexpr (DescriptorT::is_static)
                return const_cast<MemberT*>(&DescriptorT::get());
            else
                return const_cast<MemberT*>(&DescriptorT::get(obj));
        }(),
        .windowInfo = windowInfo
    };

    ImGui::PushID(metadata.obj);

    DisplaySimpleType<ReflectT, MemberT, DescriptorT>(metadata);

    // Check for a tooltip
    CheckDisplayTooltip<ReflectT, MemberT, DescriptorT>(metadata);

    ImGui::PopID();
}

template <typename ReflectT, typename MemberT, typename DescriptorT>
void TypeRenderer::DisplaySimpleType(const Metadata<ReflectT, MemberT, DescriptorT>& metadata)
{
    TypeRendererImpl<MemberT>::template Render<ReflectT, DescriptorT>(metadata);
}

template <typename ReflectT, typename MemberT, typename DescriptorT>
void TypeRenderer::CheckDisplayTooltip(const Metadata<ReflectT, MemberT, DescriptorT>&)
{
    // Check if it has the attribute
    if constexpr (Reflection::HasAttribute<Reflection::Tooltip, DescriptorT>())
    {
        // Set tooltip
        ImGui::SetItemTooltip("%s", Reflection::GetAttribute<Reflection::Tooltip, DescriptorT>().text);
    }
}

END_XNOR_CORE
