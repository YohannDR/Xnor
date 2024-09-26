﻿#pragma once

#include <refl/refl.hpp>
#include "core.hpp"

/// @file reflection.hpp
/// @brief Defines reflection types and helpers

BEGIN_XNOR_CORE

/// @brief refl::type_descriptor<T> shorthand
/// @tparam T Type
template <typename T>
using TypeDescriptor = refl::type_descriptor<T>;

/// @brief refl::attr::usage::type shorthand
///
/// Specifies that an attribute can only be used on a type
using TypeAttribute = refl::attr::usage::type;

/// @brief refl::attr::usage::field shorthand
///
/// Specifies that an attribute can only be used on a field
using FieldAttribute = refl::attr::usage::field;

/// @brief refl::attr::usage::function shorthand
///
/// Specifies that an attribute can only be used on a function
using FunctionAttribute = refl::attr::usage::function;

/// @brief refl::attr::usage::member shorthand
///
/// Specifies that an attribute can only be used on a member (field and function)
using MemberAttribute = refl::attr::usage::member;

/// @brief Macro used to implement reflection for a class
#define REFLECTABLE_IMPL(type)                        \
private:                                              \
friend struct refl_impl::metadata::type_info__<type>; \

class Component;

/// @brief Provides utility functions for reflection
namespace Reflection
{
    /// @brief Specifies that the field shouldn't be serialized
    struct NotSerializable : FieldAttribute
    {
    };

    /// @brief Specifies that the field shouldn't be serialized as a Guid, but that the values behind the pointer should be serialized instead
    struct ExpandPointer : FieldAttribute
    {
    };

    /// @brief Specifies that the field shouldn't be displayed in the inspector
    struct HideInInspector : FieldAttribute
    {
    };

    /// @brief Allows a boolean to be set to true when the field is modified
    /// @tparam T Parent type
    template <typename T>
    struct NotifyChange : FieldAttribute
    {
        /// @brief Shorthand for a class member pointer
        using PtrType = bool_t T::*;

        /// @brief Pointer to the boolean
        PtrType pointer;

        /// @brief Creates a notify change attribute using a pointer to a boolean inside the concerned class
        /// @param ptr Boolean pointer in the class
        constexpr explicit NotifyChange(const PtrType ptr) : pointer(ptr) {}
    };

    template <typename T>
    struct ModifiedCallback : FieldAttribute
    {
        using Type = void(*)(T*);
        
        Type callback;

        constexpr explicit ModifiedCallback(Type&& c) : callback(std::move(c)) {}
    };

    /// @brief Allows an enum to be treated as a list of binary flags
    struct EnumFlags : FieldAttribute
    {
    };

    /// @brief Allows an integer or floating type to be bound between a minimum and a maximum value, it will display the field using a slider
    /// @tparam T Field type
    template <typename T>
    struct Range : FieldAttribute
    {
        /// @brief Minimum value
        T minimum;
        /// @brief Maximum value
        T maximum;

        /// @brief Creates a range
        /// @param min Minimum
        /// @param max Maximum
        constexpr explicit Range(const T& min, const T& max) : minimum(min), maximum(max) {}
    };

    /// @brief Allows an integer or floating type to be bound between a dynamic minimum and a maximum value, it will display the field using a slider
    /// @tparam ReflectT Top level type
    /// @tparam T Field type
    template <typename ReflectT, typename T>
    struct DynamicRange : FieldAttribute
    {
        /// @brief Shorthand for a class member pointer
        using PtrType = T ReflectT::*;

        /// @brief Minimum value
        PtrType minimum;
        /// @brief Maximum value
        PtrType maximum;

        /// @brief Creates a dynamic range with 0 being the minimum
        /// @param max Maximum
        constexpr explicit DynamicRange(const PtrType max) : minimum(nullptr), maximum(max) {}

        /// @brief Creates a dynamic range
        /// @param min Minimum
        /// @param max Maximum
        constexpr explicit DynamicRange(const PtrType min, const PtrType max) : minimum(min), maximum(max) {}
    };

    /// @brief Allows a tooltip to be bound to a field
    struct Tooltip : FieldAttribute
    {
        /// @brief Tooltip text
        const char_t* text;

        /// @brief Creates a tooltip from a string literal
        /// @param t Tooltip text
        constexpr explicit Tooltip(const char_t* const t) : text(t) {}
    };

    /// @brief Specifies that a Vector3 should be displayed as euler angles
    struct AsEulerAngles : FieldAttribute
    {
    };

    struct AsAngle : FieldAttribute
    {
    };

    /// @brief Allows to edit a Vector2 by plotting on a 2D grid
    struct GridPlotting : FieldAttribute
    {
        /// @brief Minimum value
        float_t minimum;
        /// @brief Maximum value
        float_t maximum;

        /// @brief Creates a grid plotting from a min/max value
        /// @param min Minimum
        /// @param max Maximum
        constexpr explicit GridPlotting(const float_t min, const float_t max) : minimum(min), maximum(max) {}
    };

    /// @brief Display a field as read only
    struct ReadOnly : FieldAttribute
    {
    };

    /// @brief Binds a window to a type
    struct OpenEditorWindow : TypeAttribute
    {
        /// @brief Window text
        const char_t* windowName;

        /// @brief Creates a tooltip from a string literal
        /// @param name Window name
        constexpr explicit OpenEditorWindow(const char_t* const name) : windowName(name) {}
    };

    /// @brief Prevents a field from being cloned when cloning a type
    struct DontClone : FieldAttribute
    {
    };

    /// @brief Prevents a field content from being displayed
    struct DontExpand : FieldAttribute
    {
    };

    template <typename T>
    constexpr bool_t IsReflected = refl::trait::is_reflectable_v<T>;

    template <typename T>
    constexpr bool_t IsFunction = refl::trait::is_function_v<T>;
    
    /// @brief Gets the type info of a class
    /// @tparam ReflectT Type
    /// @return Type info
    template <typename ReflectT>
    constexpr TypeDescriptor<ReflectT> GetTypeInfo();

    /// @brief Checks if a descriptor has a specified attribute
    /// @tparam AttributeT Attribute type
    /// @tparam DescriptorT Descriptor type
    /// @return Result
    template <typename AttributeT, typename DescriptorT>
    constexpr bool_t HasAttribute();

    /// @brief Gets the specified attribute of a descriptor
    /// @tparam AttributeT Attribute type
    /// @tparam DescriptorT Descriptor type
    /// @return Attribute
    template <typename AttributeT, typename DescriptorT>
    constexpr const AttributeT& GetAttribute();

    /// @brief Tries to get a specified attribute of a descriptor
    /// @tparam AttributeT Attribute type
    /// @tparam DescriptorT Descriptor type
    /// @return Attribute, @c nullptr if not found
    template <typename AttributeT, typename DescriptorT>
    constexpr const AttributeT* TryGetAttribute();

    template <typename T>
    void Clone(const T* src, T* dst);

    void CloneUsingFactory(const void* src, void* dst, size_t hash);
    [[nodiscard]]
    void* CreateUsingFactory(size_t hash, const Component* obj);
}

END_XNOR_CORE

#include "reflection/reflection.inl"
