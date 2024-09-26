#pragma once

#include "bloom_pass.hpp"
#include "core.hpp"
#include "tone_mapping.hpp"
#include "rendering/frame_buffer.hpp"
#include "..\post_process_render_target.hpp"
#include "resource/compute_shader.hpp"

/// @file post_process_pass.hpp
/// @brief Defines the XnorCore::PostProcessPass class

BEGIN_XNOR_CORE

/// @brief Handles the post process pass
class PostProcessPass
{
public:
    XNOR_ENGINE PostProcessPass() = default;
    XNOR_ENGINE ~PostProcessPass();
    
    DEFAULT_COPY_MOVE_OPERATIONS(PostProcessPass)

    /// @brief Initializes the post process pass
    XNOR_ENGINE void Init();

    /// @brief Computes the bloom pass
    /// @param textureToCompute Image to apply post process to
    /// @param postProcessedTexture Image to merge with
    /// @param postProcessRendertarget Post process render target
    XNOR_ENGINE void Compute(const Texture& textureToCompute, const Texture& postProcessedTexture, const PostProcessRenderTarget& postProcessRendertarget) const;

private:
    Framebuffer* m_FrameBuffer = nullptr;
    ToneMapping m_ToneMapping;
    BloomPass m_BloomPass;
    RenderPass m_RenderPass;
};

END_XNOR_CORE;
