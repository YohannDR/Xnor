// ReSharper disable CppClangTidyBugproneMacroParentheses
#pragma once

#include <cmath>
#include <cstdint>

/// @file core.hpp
/// @brief This file is meant to be included by every single other header file of this project.
///
/// It includes the standard headers @c \<cmath> and @c \<cstdint> so that types such as
/// @c int8_t or @c float_t are defined. It also defines typedefs for @ref char_t and
/// @ref bool_t, as they aren't defined by those standard headers.
///
/// Apart from typedefs, this file declares macros ranging from necessary (@ref XNOR_ENGINE)
/// to useful and of general-use (@ref DEFAULT_COPY_MOVE_OPERATIONS).

// ReSharper disable once CppEnforceTypeAliasCodeStyle
/// @brief Equivalent to <c>char</c>.
/// 
/// ### Reason
/// We use a typedef here instead of a type alias for consistency
/// with how the other integral types are defined in the <c>cstdint</c> header.
/// Also, we need to manually add this type because it is considered
/// different to <c>int8_t</c> as the latter is the equivalent of <c>signed char</c>
/// and not <c>char</c>.
typedef char char_t;

// ReSharper disable once CppEnforceTypeAliasCodeStyle
/// @brief Equivalent to <c>bool</c>.
///
/// @see @ref char_t for reason.
typedef bool bool_t;

/// @brief Macro used for DLL export/import.
///
/// This macro should be used at the beginning of static member variable and non-inline function declarations.
#if defined(XNOR_EXPORT)
#define XNOR_ENGINE __declspec(dllexport)
#else
#define XNOR_ENGINE __declspec(dllimport)
#endif

#define ALLOCATOR_FUNC __declspec(allocator)

/// @brief Starts the @ref XnorCore namespace.
#define BEGIN_XNOR_CORE namespace XnorCore {

/// @brief Ends the @ref XnorCore namespace.
#define END_XNOR_CORE }

/// @namespace XnorCore
/// @brief Contains all declarations of the XNOR Engine.
namespace XnorCore {}

#ifndef SWIG
/// @brief Creates default copy and move operations for a given @p type.
///
/// ### Usage
/// This macro should be used for any type that defines at least one of:
/// - A non-default destructor (this includes default implementations),
/// - A non-default copy constructor
/// - A non-default move constructor
/// - A non-default copy operator
/// - A non-default move operator
/// 
/// It should be used right after the last constructor declaration:
/// @code
/// class A
/// {
/// public:
///     A();
///     
///     A(int);
///
///     DEFAULT_COPY_MOVE_OPERATIONS(A)
/// };
/// @endcode
/// Or after the destructor declaration if there is one:
/// @code
/// class A
/// {
/// public:
///     A();
///     
///     A(int);
///
///     ~A();
///
///     DEFAULT_COPY_MOVE_OPERATIONS(A)
/// };
/// @endcode
/// 
/// @param type The type to default the copy and move operations of.
#define DEFAULT_COPY_MOVE_OPERATIONS(type)         \
    type(const type& other) noexcept = default;             \
    type(type&& other) noexcept = default;                  \
    type& operator=(const type& other) noexcept = default;  \
    type& operator=(type&& other) noexcept = default;

/// @brief Deletes the copy and move operations of a given @p type.
/// 
/// @param type The type to delete the copy and move operations of.
///
/// @see @ref DEFAULT_COPY_MOVE_OPERATIONS for usage.
#define DELETE_COPY_MOVE_OPERATIONS(type)          \
    type(const type& other) = delete;              \
    type(type&& other) = delete;                   \
    type& operator=(const type& other) = delete;   \
    type& operator=(type&& other) = delete;
#endif

/// @brief Macro used to declare a static class, e.g. a class that cannot be instantiated.
/// 
/// For consistency, this should be the first statement inside of the class definition:
/// @code
/// class A
/// {
///     STATIC_CLASS(A)
///     
/// public:
///     ...
/// };
/// @endcode
///
/// Also, a static class shouldn't define any other constructors.
/// 
/// @param type The type to make static.
#define STATIC_CLASS(type)              \
    public:                             \
    type() = delete;                    \
    ~type() = delete;                   \
    DELETE_COPY_MOVE_OPERATIONS(type)   \
    private:

// We need this in order to be able to make Doxygen links to std::string conversion operators.
#ifdef DOXYGEN
/// @private
using stdstring = std::string;
#endif

/// @brief Opens an @c enum with the given name.
///
/// ### Reason
/// Since using an @c enum @c class type means casting a value every time we want it as a number, and
/// using a classic C @c enum means having its constants in the current namespace, we use this set of macros instead.
///
/// This macro generates a @c namespace with the given enum name, containing a C @c enum with the given name in it.
/// This means that all the @c enum constants are in a separate namespace and should therefore be accessed with the enum name
/// each time, but also that for storing a value of that type we need to use @c EnumName::EnumName. To fix that, we created
/// the ENUM_VALUE macro, which effectively does what we need.
///
/// ### Usage
/// This macro should be used as the following:
/// @code
/// BEGIN_ENUM(EnumName)
/// {
///     Value1,
///     Value2,
///     Value3
/// }
/// END_ENUM
/// @endcode
///
/// A variable storing a value of an @c enum generated with this macro should be declared like this:
/// @code
/// ENUM_VALUE(EnumName) variableName = EnumName::Value2;
/// @endcode
#ifndef DOXYGEN
#define BEGIN_ENUM(name)    \
    namespace name          \
    {                       \
        enum name
#else
#define BEGIN_ENUM(name)    \
    enum name
#endif

/// @brief Closes an @c enum declaration that was opened using BEGIN_ENUM.
///
/// @see BEGIN_ENUM
#ifndef DOXYGEN
#define END_ENUM    \
        ;           \
    }
#else
#define END_ENUM    \
    ;
#endif

/// @brief Generates the type for a variable using an @c enum that was created with the BEGIN_ENUM and END_ENUM macros.
///
/// @see BEGIN_ENUM
#ifndef DOXYGEN
#define ENUM_VALUE(enumName) enumName::enumName
#else
#define ENUM_VALUE(enumName) enumName
#endif

#if defined(SWIG) || defined(SWIG_WRAP)
#define SWIG_ONLY
#endif

/// @brief Equivalent to @c alignas(size).
///
/// We need this to be a macro because it generates SWIG syntax errors.
#ifndef SWIG
#define ALIGNAS(size) alignas(size)
#else
#define ALIGNAS(size)
#endif

/// @brief Forwards a value with its own type as template.
#define FORWARD(value) std::forward<decltype(value)>(value)
