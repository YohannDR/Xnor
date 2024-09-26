#pragma once

#include "core.hpp"
#include "render_pass.hpp"
#include "resource/texture.hpp"
#include "frame_buffer.hpp"

/// @file bloom_render_target.hpp
/// @brief Defines the XnorCore::BloomRenderTarget

BEGIN_XNOR_CORE

/// @brief Defines information for a bloom render target
class BloomRenderTarget
{
    static inline constexpr TextureInternalFormat::TextureInternalFormat BloomTextureFormat = TextureInternalFormat::Rgba32F;
    static inline constexpr uint32_t BloomMipNumber = 5;

public:
    /// @brief Bloom mip map info
    struct BloomMip
    {
        /// @brief Bloom mip map texture
        Texture* texture = nullptr;
        /// @brief Bloom mip map size
        Vector2 sizef;
    };

    /// @brief Bloom mip map
    std::vector<BloomMip> mipChain;
    /// @brief Bloom threshold texture
    Texture* thresholdTexture = nullptr;

    /// @brief Bloom framebuffer
    Framebuffer* frameBuffer = nullptr;
    /// @brief Bloom render pass
    RenderPass renderPass;
    
    XNOR_ENGINE BloomRenderTarget();
    
    XNOR_ENGINE ~BloomRenderTarget();

    DEFAULT_COPY_MOVE_OPERATIONS(BloomRenderTarget)

    /// @brief Initializes the bloom render target
    /// @param viewportSize Size
    XNOR_ENGINE void Init(Vector2i viewportSize);

    /// @brief Destroys the bloom render target
    XNOR_ENGINE void Destroy();

    /// @brief Resizes the bloom render target
    /// @param viewportSize Size
    XNOR_ENGINE void Resize(Vector2i viewportSize);

    /// @brief Gets the resulting bloomed texture
    /// @returns Bloomed texture
    [[nodiscard]]
    XNOR_ENGINE Texture* GetBloomedTexture() const;

    /// @brief Creates the bloom mip map
    /// @param viewportSize Size
    XNOR_ENGINE void CreateBloomMip(Vector2i viewportSize);
};

END_XNOR_CORE
