// ReSharper disable CppClangTidyCppcoreguidelinesMacroUsage

// ReSharper disable CppClangTidyCppcoreguidelinesAvoidCArrays
#pragma once

#include "core.hpp"

#include "resource/resource_manager.hpp"
#include "scene/entity.hpp"
#include "scene/component/script_component.hpp"

#include "utils/meta_programming.hpp"

/// @file type_renderer.hpp
/// @brief Defines the TypeRenderer

BEGIN_XNOR_CORE

/// @brief Helper class to render an object using compile time reflection
class TypeRenderer
{
    STATIC_CLASS(TypeRenderer)
    
public:
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

        /// @brief Window opening info, contains a pointer to the selected element and the name of the window to open
        std::pair<void*, const char_t*>* windowInfo;
    };

    /// @brief Displays an object
    /// @tparam ReflectT Object type
    /// @param obj Object pointer
    /// @param windowInfo Window information, pass nullptr
    /// @returns Window information
    template <typename ReflectT>
    static std::pair<void*, const char_t*> DisplayObject(ReflectT* obj, std::pair<void*, const char_t*>* windowInfo = nullptr);
    
    /// @brief Displays a grid plotting for a Vector2
    /// @tparam ReflectT Reflected top level type
    /// @tparam MemberT Member type
    /// @tparam DescriptorT Field descriptor type
    /// @param metadata Member metadata
    template <typename ReflectT, typename MemberT, typename DescriptorT>
    static void DisplayGridPlotting(const Metadata<ReflectT, MemberT, DescriptorT>& metadata);

    /// @brief Displays an enum
    /// @tparam ReflectT Reflected top level type
    /// @tparam MemberT Member type
    /// @tparam DescriptorT Field descriptor type
    /// @param metadata Member metadata
    template <typename ReflectT, typename MemberT, typename DescriptorT>
    static void DisplayEnum(const Metadata<ReflectT, MemberT, DescriptorT>& metadata);

    /// @brief Displays an enum flag
    /// @tparam ReflectT Reflected top level type
    /// @tparam MemberT Member type
    /// @tparam DescriptorT Field descriptor type
    /// @param metadata Member metadata
    template <typename ReflectT, typename MemberT, typename DescriptorT>
    static void DisplayEnumFlag(const Metadata<ReflectT, MemberT, DescriptorT>& metadata);

    /// @brief Displays a simple type (calls its TypeRendererImpl)
    /// @tparam ReflectT Reflected top level type
    /// @tparam MemberT Member type
    /// @tparam DescriptorT Field descriptor type
    /// @param metadata Member metadata
    template <typename ReflectT, typename MemberT, typename DescriptorT>
    static void DisplaySimpleType(const Metadata<ReflectT, MemberT, DescriptorT>& metadata);

    /// @brief Displays an object via the Factory using its hash
    /// @param obj Object pointer
    /// @param hash Object hash
    /// @param windowInfo Window info
    XNOR_ENGINE static void DisplayObjectUsingFactory(void* obj, size_t hash, std::pair<void*, const char_t*>* windowInfo);

private:
    template <typename ReflectT, typename MemberT, typename DescriptorT>
    static void DisplayObjectInternal(ReflectT* obj, std::pair<void*, const char_t*>* windowInfo);

    template <typename ReflectT, bool_t StaticT>
    static void DisplayFields(ReflectT* obj, std::pair<void*, const char_t*>* windowInfo);

    /// @brief Displays a tooltip if the member has one
    /// @tparam ReflectT Reflected top level type
    /// @tparam MemberT Member type
    /// @tparam DescriptorT Field descriptor type
    /// @param metadata Member metadata
    template <typename ReflectT, typename MemberT, typename DescriptorT>
    static void CheckDisplayTooltip(const Metadata<ReflectT, MemberT, DescriptorT>& metadata);
};

/// @brief Implementation for a type renderer, template specialization can be used to provide a custom render behavior to a custom type
/// @tparam MemberT Member type
template <typename MemberT, typename = void>
struct TypeRendererImpl
{
    /// @brief Renders the provided type
    /// @tparam ReflectT Reflected top level type
    /// @tparam DescriptorT Field descriptor type
    /// @param metadata Member metadata
    template <typename ReflectT, typename DescriptorT>
    static void Render(const TypeRenderer::Metadata<ReflectT, MemberT, DescriptorT>& metadata);
};

END_XNOR_CORE

#include "reflection/type_renderer.inl"
#include "reflection/type_renderer_impl.inl"
