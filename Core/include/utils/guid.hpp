#pragma once

#include <array>
#include <cstdint>
#include <format>
#include <sstream>
#include <string>

#include "core.hpp"

/// @file guid.hpp
/// @brief Defines the XnorCore::Guid class.

BEGIN_XNOR_CORE

/// @brief Stands for Global Unique Identifier, it represents a unique ID that's used to link pointers during serialization and deserialization
class XNOR_ENGINE Guid final
{
    static constexpr size_t Data4Size = 8;
    
public:
    constexpr Guid() = default;

    /// @brief Empty guid
    static constexpr Guid Empty() { return Guid(); }
    
    /// @brief Creates a new @ref Guid
    /// @return New guid
    static Guid New();

    /// @brief Parses a Guid from a string
    /// @param str String
    /// @returns Guid
    static Guid FromString(const char_t* str);
    
    [[nodiscard]]
    uint32_t GetData1() const;
    
    [[nodiscard]]
    uint16_t GetData2() const;
    
    [[nodiscard]]
    uint16_t GetData3() const;
    
    [[nodiscard]]
    const std::array<uint8_t, Data4Size>& GetData4() const;

    /// @brief Compares 2 @ref Guid
    /// @param other Other guid
    /// @return Whether the @ref Guid are equal
    [[nodiscard]]
    bool_t operator==(const Guid& other) const;

    /// @brief Compares 2 @ref Guid
    /// @param other Other guid
    /// @return Whether the @ref Guid are different
    [[nodiscard]]
    bool_t operator!=(const Guid& other) const;

    /// @brief Converts a @ref Guid to a string representation
    explicit operator std::string() const;

private:
    uint32_t m_Data1 = 0;
    uint16_t m_Data2 = 0;
    uint16_t m_Data3 = 0;
    std::array<uint8_t, Data4Size> m_Data4 = {};

    friend struct std::hash<Guid>;
};

END_XNOR_CORE

/// @private
#ifndef SWIG
template <>
struct std::hash<XnorCore::Guid>
{
    static constexpr inline size_t RandomValue = 0x9E3779B9;
    
    size_t operator()(const XnorCore::Guid& guid) const noexcept
    {
        size_t result = 0;
        result ^= std::hash<decltype(guid.m_Data1)>()(guid.m_Data1) + RandomValue;
        result ^= std::hash<decltype(guid.m_Data2)>()(guid.m_Data2) + RandomValue + (result << 6) + (result >> 2);
        result ^= std::hash<decltype(guid.m_Data3)>()(guid.m_Data3) + RandomValue + (result << 6) + (result >> 2);

        for (size_t i = 0; i < XnorCore::Guid::Data4Size; i++)
        {
            result ^= std::hash<std::remove_cvref_t<decltype(guid.m_Data4[i])>>()(guid.m_Data4[i]) + RandomValue + (result << 6) + (result >> 2);
        }

        return result;
    }
};

/// @brief @c std::formatter template specialization for the XnorCore::Guid type.
template <>
struct std::formatter<XnorCore::Guid>
{
    /// @brief Parses the input formatting options.
    template <class ParseContext>
    constexpr typename ParseContext::iterator parse(ParseContext& ctx)
    {
        auto it = ctx.begin();
        if (it == ctx.end())
            return it;
 
        if (*it != '}')
            throw std::format_error("Invalid format args for XnorCore::Guid");
 
        return it;
    }

    // ReSharper disable once CppMemberFunctionMayBeStatic
	/// @brief Formats a string using the given instance of XnorCore::Guid, according to the given options in the parse function.
    template <class FormatContext>
    typename FormatContext::iterator format(const XnorCore::Guid& guid, FormatContext& ctx) const
    {
        std::ostringstream out;

        out << static_cast<std::string>(guid);
        
        return std::ranges::copy(std::move(out).str(), ctx.out()).out;
    }
};
#endif
