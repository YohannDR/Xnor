#pragma once

#include "core.hpp"
#include "resource/model.hpp"
#include "resource/shader.hpp"
#include "utils/pointer.hpp"
#include "world/skybox.hpp"

/// @file skybox_renderer.hpp
/// @brief Defines the XnorCore::SkyboxRenderer class

BEGIN_XNOR_CORE

/// @brief Renderer specialized for a skybox
class SkyboxRenderer
{
private:
    static inline constexpr uint32_t SkyBoxAlbedoTextureBindingIndex = 0;
public:
    XNOR_ENGINE SkyboxRenderer() = default;
    
    XNOR_ENGINE ~SkyboxRenderer() = default;

    DEFAULT_COPY_MOVE_OPERATIONS(SkyboxRenderer)

    /// @brief Initializes the shader resources
    XNOR_ENGINE void InitializeResources();

    /// @brief Renders a skybox
    /// @param cubeModel Cube model
    /// @param skybox Skybox
    XNOR_ENGINE void DrawSkymap(const Pointer<Mesh>& cubeModel, const Skybox& skybox) const;
  
private:
    Pointer<Shader> m_SkyboxDrawerShader;
};

END_XNOR_CORE
