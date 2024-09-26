#include "resource/font.hpp"

#include "utils/logger.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

#include "rendering/rhi.hpp"

using namespace XnorCore;

Font::~Font()
{
    if (m_LoadedInInterface)
        Font::DestroyInInterface();
    
    if (m_Loaded)
        Font::Unload();
}

void Font::CreateInInterface()
{
    FT_Library ft = nullptr;
    if (FT_Init_FreeType(&ft))
    {
        Logger::LogError("Error freetype : Could not init FreeType Library");
        return;
    }

    FT_Face face = nullptr;
    const std::string stringPath = m_File->GetPath().generic_string();
    if (FT_New_Face(ft, stringPath.c_str(), 0, &face))
    {
        Logger::LogError("Error freetype : Failed to load font");
        return;
    }

    FT_Set_Pixel_Sizes(face, 0, 48);  
   
    Rhi::SetPixelStore(DataAlignment::UnPack, 1);
    
    for (uint8_t c = 0; c < 128; c++)
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            Logger::LogError("Error freetype : Failed to load Glyph");
            continue;
        }

        const Vector2i glyphSize = Vector2i(static_cast<int32_t>(face->glyph->bitmap.width), static_cast<int32_t>(face->glyph->bitmap.rows));
        
      

        TextureCreateInfo createInfo =
        {
            .textureType = TextureType::Texture2D,
            .datas = { face->glyph->bitmap.buffer },
            .mipMaplevel = 1,
            .depth = 1,
            .size = glyphSize,
            .filtering = TextureFiltering::Linear,
            .wrapping = TextureWrapping::ClampToEdge,
            .format = TextureFormat::Red,
            .internalFormat = TextureInternalFormat::R8,
            .dataType = DataType::UnsignedByte,
        };

        Character character;
        character.texture = new Texture(createInfo);
        character.size = Vector2i(static_cast<int32_t>(face->glyph->bitmap.width), static_cast<int32_t>(face->glyph->bitmap.rows));
        character.bearing = Vector2i(face->glyph->bitmap_left, face->glyph->bitmap_top);
        character.advance = face->glyph->advance.x;
        m_Characters.emplace(c, character);
    }
    
    Rhi::SetPixelStore(DataAlignment::UnPack, 4);

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
    m_LoadedInInterface = true;
}

void Font::DestroyInInterface()
{
    for (decltype(m_Characters)::iterator it = m_Characters.begin(); it != m_Characters.end(); it++)
    {
        delete it->second.texture;
    }

    m_LoadedInInterface = false;
}

const Font::Character& Font::GetGlyphByChar(const char_t characters) const 
{
    return m_Characters.at(characters);
}
