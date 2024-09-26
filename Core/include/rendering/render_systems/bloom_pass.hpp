#pragma once

#include "core.hpp"
#include "..\bloom_render_target.hpp"
#include "resource/compute_shader.hpp"
#include "resource/model.hpp"
#include "resource/shader.hpp"

/// @file bloom_pass.hpp
/// @brief Defines the XnorCore::BloomPass class

BEGIN_XNOR_CORE

/// @brief Handles the bloom pass
class BloomPass
{
    static inline constexpr TextureInternalFormat::TextureInternalFormat BloomTextureFormat = TextureInternalFormat::Rgba32F;
    static inline constexpr float_t ComputeShaderDispactValue = 8;

public:
    DEFAULT_COPY_MOVE_OPERATIONS(BloomPass)

    XNOR_ENGINE BloomPass() = default;
    XNOR_ENGINE ~BloomPass() = default;

    /// @brief Initializes the bloom pass
    XNOR_ENGINE void Init();

    /// @brief Computes the bloom pass
    /// @param imageWithoutBloom Image to apply bloom to
    /// @param bloomRenderTarget Bloom render target
    XNOR_ENGINE void ComputeBloom(const Texture& imageWithoutBloom, const BloomRenderTarget& bloomRenderTarget) const;
    
private:
    Pointer<Model> m_Quad = nullptr;
    Pointer<ComputeShader> m_DownSample = nullptr;
    Pointer<ComputeShader> m_UpSample = nullptr;
    Pointer<ComputeShader> m_ThresholdFilter = nullptr;

    XNOR_ENGINE void UpSampling(const BloomRenderTarget& bloomRenderTarget) const;
    
    XNOR_ENGINE void DownSampling(const BloomRenderTarget& bloomRenderTarget) const;
    
    XNOR_ENGINE void ThresholdFilter(const Texture& imageWithoutBloom, const BloomRenderTarget& bloomRenderTarget) const;
};

END_XNOR_CORE
