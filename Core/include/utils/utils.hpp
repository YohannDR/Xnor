﻿#pragma once

#include <filesystem>
#include <functional>

#include <Maths/quaternion.hpp>
#include <Maths/vector2.hpp>
#include <Maths/vector3.hpp>

#include "core.hpp"

#include "utils/concepts.hpp"
#include "utils/pointer.hpp"

/// @file utils.hpp
/// @brief Defines general utility functions.

BEGIN_XNOR_CORE

class File;
class Directory;
class Entry;

/// @namespace Utils
/// @brief Namespace containing utility functions that don't belong anywhere else
namespace Utils
{
    /// @brief Converts a integral number to a valid pointer without illegal size operations
    /// @tparam PtrT Type of the pointer, must be a pointer type
    /// @tparam IntT Type of the number, must be integral
    /// @param number Number to convert
    /// @return Pointer representation of the number
    template <Concepts::PointerT PtrT, Concepts::IntegralT IntT>
    [[nodiscard]]
    constexpr PtrT IntToPointer(IntT number);

    /// @brief Gets the hash code of a specified type
    /// @tparam T Type
    /// @return Hash
    template <typename T>
    [[nodiscard]]
    size_t GetTypeHash();

    /// @brief Gets the hash code of a specified polymorphic pointer type
    /// @tparam T Type
    /// @param ptr Polymorphic pointer
    /// @return Hash
    template <typename T>
    [[nodiscard]]
    size_t GetTypeHash(const T* ptr);

    /// @brief Horizontally aligns the cursor of ImGui to be centered around a specific portion of the available space
    /// @param objectWidth Width of the element to align
    /// @param alignment In window alignment, 0.5f by default to center the object
    XNOR_ENGINE void AlignImGuiCursor(float_t objectWidth, float_t alignment = 0.5f);

    /// @brief Humanizes the provided string
    ///
    /// The process converts a PascalCase styled word to a humanized version that puts spaces between each words and adds an uppercase at the very beginning
    ///
    /// e.g. RequiresUIReloadAttribute will become Requires UI Reload Attribute
    /// @param str String to humanize
    /// @return Result
    [[nodiscard]]
    XNOR_ENGINE std::string HumanizeString(const std::string& str);

    /// @brief Humanizes the provided variable name
    /// 
    /// The process converts a m_PascalCase styled word to a humanized version that puts spaces between each words and adds an uppercase at the very beginning, it also removes the m_ prefix
    ///
    /// e.g. m_ShouldChange will become Should Change
    /// @param str String to humanize
    /// @return Result
    [[nodiscard]]
    XNOR_ENGINE std::string HumanizeVariableName(const std::string& str);

    /// @brief Removes the namespaces indicators from the provided string
    ///
    /// e.g. XnorCore::MyClass will become MyClass
    /// 
    /// @param str String to modify
    /// @return Result
    [[nodiscard]]
    XNOR_ENGINE constexpr std::string RemoveNamespaces(const std::string& str);

#ifndef SWIG
    /// @brief Removes the namespaces indicators from the provided string
    ///
    /// e.g. XnorCore::MyClass will become MyClass
    /// 
    /// @param str String to modify
    /// @return Result
    [[nodiscard]]
    XNOR_ENGINE constexpr const char_t* RemoveNamespaces(const char_t* str);
#endif

    /// @brief Remaps a value from one range to another
    ///
    /// e.g., the number 5 in the range [0;10] will become .5 if remapped to the range [0;1]
    /// @param oldValue Value
    /// @param oldRange Old range
    /// @param newRange New range
    /// @return New value
    [[nodiscard]]
    XNOR_ENGINE constexpr float_t RemapValue(float_t oldValue, Vector2 oldRange, Vector2 newRange);

    /// @brief Remaps a value from one range to another
    ///
    /// e.g., the number 5 in the range [0;10] will become 1 if remapped to the range [0;2]
    /// @param oldValue Value
    /// @param oldRange Old range
    /// @param newRange New range
    /// @return New value
    [[nodiscard]]
    XNOR_ENGINE constexpr size_t RemapValue(size_t oldValue, Vector2i oldRange, Vector2i newRange);

    /// @brief Normalizes an angle (clamps its value between 0 and 2 * PI)
    /// @param angle Angle to normalize
    /// @return Normalized representation
    XNOR_ENGINE float_t NormalizeAngle(float_t angle);

    /// @brief Normalizes a set of 3 angles in a Vector3 (clamps their value between 0 and 2 * PI)
    /// @param angles Vector3 of angles to normalize
    /// @return Normalized representation
    XNOR_ENGINE Vector3 NormalizeAngles(Vector3 angles);

    /// @brief Converts a quaternion to its euler angle representation
    /// @param rot Quaternion to convert
    /// @return Euler representation
    XNOR_ENGINE Vector3 GetQuaternionEulerAngles(const Quaternion& rot);

    /// @brief Equivalent of a @c dynamic_cast for Pointer "Pointers".
    ///
    /// This function first checks if the given Pointer is null, and returns null if so.
    /// It then checks if one type is derived from the other, and returns the casted result if so. This actually performs a @c reinterpret_cast under the hood.
    /// If all other conditions failed, the function returns a null Pointer.
    /// 
    /// @tparam T The type to dynamically cast to.
    /// @tparam U The type to dynamically cast from.
    /// @param value The Pointer to cast from.
    /// @return A null Pointer if the cast failed. Otherwise, the cast result.
    template <typename T, typename U>
    [[nodiscard]]
    Pointer<T> DynamicPointerCast(const Pointer<U>& value);

    /// @brief Opens the specified path in the file explorer
    /// @param path File system path
    XNOR_ENGINE void OpenInExplorer(const std::filesystem::path& path);

    /// @brief Opens the specified path in the file explorer
    /// @param path File system path
    /// @param isFile Whether path is a file or a directory
    XNOR_ENGINE void OpenInExplorer(const std::filesystem::path& path, bool_t isFile);

    /// @brief Opens the specified file on the user's computer
    /// @param filepath File system path
    XNOR_ENGINE void OpenFile(const std::filesystem::path& filepath);

    /// @brief Returns whether an array contains an element
    template <std::ranges::input_range Container, typename T>
    [[nodiscard]]
    bool_t ArrayContains(const Container& container, T element);

    /// @brief Returns whether a string array contains an element using Utils::StringEqualsIgnoreCase.
    template <std::ranges::input_range Container>
    [[nodiscard]]
    bool_t StringArrayContains(const Container& container, const std::string& element);

    /// @brief Checks if two strings are equal, case-insensitive.
    [[nodiscard]]
    XNOR_ENGINE bool_t StringEqualsIgnoreCase(const std::string& a, const std::string& b);

    /// @brief Gets the address of a function
    /// @tparam Ret Function return type
    /// @tparam Args Function arguments types
    /// @param f Function
    /// @return Address
    template <typename Ret, typename... Args>
    [[nodiscard]]
    constexpr size_t FunctionAddress(std::function<Ret(Args...)> f);

    XNOR_ENGINE int32_t TerminalCommand(const std::string& command, bool_t asynchronous = true);

    XNOR_ENGINE void CreateEmptyFile(const std::filesystem::path& path);

    XNOR_ENGINE void SetThreadName(std::thread& thread, const std::wstring& name);
}

END_XNOR_CORE

#include "utils/utils.inl"
