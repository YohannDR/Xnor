#pragma once

#include "bloom_render_target.hpp"
#include "core.hpp"

/// @file post_process_render_target.hpp
/// @brief Defines the XnorCore::PostProcessRenderTarget

BEGIN_XNOR_CORE

/// @brief Defines information for a post process render target
class PostProcessRenderTarget
{
public:
    /// @brief Initializes the post process render target
    /// @param viewportSize Size
    XNOR_ENGINE void Init(Vector2i viewportSize);

    /// @brief Destroys the post process render target
    XNOR_ENGINE void Destroy();

    /// @brief Bloom render target
    BloomRenderTarget bloomRenderTarget;
};

END_XNOR_CORE
