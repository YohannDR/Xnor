#pragma once

#include <array>
#include <map>

#include "core.hpp"
#include "texture.hpp"
#include "resource/resource.hpp"

BEGIN_XNOR_CORE

/// @brief Holds the necessary information to draw text using a Font.
class Font : public Resource
{
public:
    /// @brief Allowed extensions for font files
    XNOR_ENGINE static constexpr std::array<const char_t*, 1> FileExtensions
    {
        ".ttf"
    };

    struct Character
    {
        Texture* texture {nullptr};
        Vector2i size;       // Size of glyph
        Vector2i bearing;    // Offset from baseline to left/top of glyph
        uint32_t advance;
    };

    DEFAULT_COPY_MOVE_OPERATIONS(Font)

    // Same constructor from base class
    using Resource::Resource;
    
    // We keep both function overloads and only override one
    using Resource::Load;
    
    XNOR_ENGINE ~Font() override;

    XNOR_ENGINE Font() = default;

    XNOR_ENGINE void CreateInInterface() override;
    
    XNOR_ENGINE void DestroyInInterface() override;

    XNOR_ENGINE const Character& GetGlyphByChar(char_t characters) const ;
    
private:
    std::map<char_t, Character> m_Characters;
};

END_XNOR_CORE

REFL_AUTO(type(XnorCore::Font, bases<XnorCore::Resource>))

