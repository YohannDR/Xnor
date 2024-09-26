#pragma once

#include <array>

#include <Maths/vector2i.hpp>

#include "core.hpp"
#include "rendering/rhi_typedef.hpp"
#include "resource/resource.hpp"

/// @file texture.hpp
/// @brief Defines the XnorCore::Texture class

BEGIN_XNOR_CORE

/// @brief Represents an image in memory.
class Texture final : public Resource
{
public:
    /// @brief Options for loading the data of a Texture.
    ///
    /// @see Texture::loadData
    struct LoadOptions
    {
        /// @brief The desired color channel count.
        ///
        /// This would be 3 for RGB, or 4 for RGBA values for example.
        /// If the value is 0, this will be determined automatically.
        int32_t desiredChannels = 0;
        /// @brief Whether to vertically flip the Texture.
        bool_t flipVertically = false;
    };

    /// @brief The default Texture loading options. This is the default value of Texture::loadData.
    XNOR_ENGINE static inline LoadOptions defaultLoadOptions;

    /// @brief Allowed extensions for texture files
    XNOR_ENGINE static constexpr std::array<const char_t*, 4> FileExtensions
    {
        ".jpg",
        ".jpeg",
        ".png",
        ".hdr"
    };

    /// @brief Options to use when loading this Texture.
    ///
    /// This doesn't affect an already-loaded Texture. If necessary, change these settings and then call Texture::Reload for the changes to take effect.
    LoadOptions loadData = defaultLoadOptions;
    
    // Same constructor from base class
    using Resource::Resource;

    DEFAULT_COPY_MOVE_OPERATIONS(Texture)

    // We keep both function overloads and only override one
    using Resource::Load;

    /// @brief Creates a texture using create info
    /// @param createInfo Creation info
    XNOR_ENGINE explicit Texture(const TextureCreateInfo& createInfo);

    /// @brief Creates a texture using a format and a size
    /// @param textureInternalFormat Format inside the FrameBuffer
    /// @param size Size
    /// @param textureFormat The memory format
    XNOR_ENGINE Texture(ENUM_VALUE(TextureInternalFormat) textureInternalFormat, Vector2i size, ENUM_VALUE(TextureFormat) textureFormat = TextureFormat::Rgb);
    
    XNOR_ENGINE ~Texture() override;

    XNOR_ENGINE bool_t Load(const uint8_t* buffer, int64_t length) override;
    
    XNOR_ENGINE void CreateInInterface() override;

    XNOR_ENGINE void DestroyInInterface() override;
    
    XNOR_ENGINE void Unload() override;

    XNOR_ENGINE void SetIsEmbedded();

    /// @brief Gets the raw data of the texture
    /// @tparam T Type
    /// @return Data
    template <typename T = char_t>
    [[nodiscard]]
    const T* GetData() const;

    /// @brief Gets the raw data of the texture
    /// @tparam T Type
    /// @return Data
    template <typename T = char_t>
    [[nodiscard]]
    T* GetData();

    /// @brief Gets the size of the texture
    /// @return Size
    [[nodiscard]]
    XNOR_ENGINE Vector2i GetSize() const;

    /// @brief Gets the number of channels of the file of the texture
    /// @return Number of channels
    [[nodiscard]]
    XNOR_ENGINE int32_t GetDataChannels() const;

    /// @brief Gets the loaded number of channels of the texture
    /// @return Number of channels
    [[nodiscard]]
    XNOR_ENGINE int32_t GetChannels() const;

    /// @brief Binds the texture
    /// @param index Index
    XNOR_ENGINE void BindTexture(uint32_t index) const;
    
    /// @brief Unbinds the texture
    /// @param index Index
    XNOR_ENGINE void UnbindTexture(uint32_t index) const;

    /// @brief Gets the texture id
    /// @return Texture id
    [[nodiscard]]
    XNOR_ENGINE uint32_t GetId() const;

    /// @brief Gets the filtering option
    /// @return Filtering
    XNOR_ENGINE ENUM_VALUE(TextureFiltering) GetTextureFiltering() const;

    /// @brief Gets the wrapping option
    /// @return Wrapping option
    XNOR_ENGINE ENUM_VALUE(TextureWrapping) GetTextureWrapping() const;
    
    /// @brief Gets the internal format
    /// @return Internal format
    XNOR_ENGINE ENUM_VALUE(TextureInternalFormat) GetInternalFormat() const;

    /// @brief Gets the format
    /// @return Format
    XNOR_ENGINE ENUM_VALUE(TextureFormat) GetTextureFormat() const;
    
    XNOR_ENGINE bool_t Save() const override;

private:
    uint8_t* m_Data = nullptr;
    Vector2i m_Size;
    int32_t m_DataChannels = 0;
    uint32_t m_Id = 0;
    bool_t m_IsEmbedded = false;

    ENUM_VALUE(TextureFiltering) m_TextureFiltering = TextureFiltering::Nearest;
    ENUM_VALUE(TextureWrapping) m_TextureWrapping = TextureWrapping::Repeat;
    ENUM_VALUE(TextureInternalFormat) m_TextureInternalFormat = TextureInternalFormat::Rgba8;
    ENUM_VALUE(TextureFormat) m_TextureFormat = TextureFormat::Rgb;
};

END_XNOR_CORE

REFL_AUTO(type(XnorCore::Texture, bases<XnorCore::Resource>))

#include "resource/texture.inl"
