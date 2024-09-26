#pragma once

#include "core.hpp"
#include "frame_buffer.hpp"
#include "post_process_render_target.hpp"
#include "render_pass.hpp"

/// @file viewport_data.hpp
/// @brief Defines the XnorCore::ViewportData class

BEGIN_XNOR_CORE

/// @brief Class that holds the necessary information for a framebuffer
class ViewportData
{
public:
    // Deferred Rendering

    /// @brief RenderPass for the G-buffer
    RenderPass gBufferPass{};
    /// @brief Framebuffer for the G-buffer
    Framebuffer* gFramebuffer = nullptr;
    /// @brief Normal vectors attachment for the G-buffer
    Texture* normalAttachement = nullptr;
    /// @brief Albedo attachment for the G-buffer
    Texture* albedoAttachment = nullptr;

    /// @brief Metallic, roughness and reflectance attachment for the G-buffer
    Texture* metallicRoughnessReflectance = nullptr;
    /// @brief Ambiant occlusion attachment for the G-buffer
    Texture* ambiantOcclusion = nullptr;
    /// @brief Emissive attachment for the G-buffer
    Texture* emissive = nullptr;

    // Forward Rendering
    /// @brief RenderPass forward buffer
    RenderPass colorPass{};
    /// @brief Framebuffer for the forward buffer
    Framebuffer* renderBuffer = nullptr;
    /// @brief Color attachment for the forward buffer
    Texture* colorAttachment = nullptr;
    /// @brief Depth attachment for the forward buffer
    Texture* depthAtttachment = nullptr;

    /// @brief Post-processing render target information
    PostProcessRenderTarget postprocessRendertarget;

    /// @brief Whether post processing is enabled
    bool_t usePostProcess = true;

    ViewportData() = default;

    ~ViewportData() = default;

    DEFAULT_COPY_MOVE_OPERATIONS(ViewportData)

    /// @brief Initializes a viewport data
    /// @param size Size
    void Init(Vector2i size);

    /// @brief Destroys the viewport data
    void Destroy();

    /// @brief Binds the descriptor
    void BindDescriptor() const;

    /// @brief Unbinds the descriptor
    void UnbindDescriptor() const;

private:
    void InitForward(Vector2i size);

    void InitDeffered(Vector2i size);
};

END_XNOR_CORE
