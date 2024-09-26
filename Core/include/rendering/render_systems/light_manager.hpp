#pragma once

#include <map>

#include "core.hpp"
#include "rendering/light/directional_light.hpp"
#include "rendering/light/point_light.hpp"
#include "rendering/light/spot_light.hpp"
#include "rendering/camera.hpp"
#include "rendering/frame_buffer.hpp"
#include "rendering/light/cascade_shadow_map.hpp"
#include "resource/model.hpp"
#include "resource/shader.hpp"
#include "resource/texture.hpp"
#include "scene/scene.hpp"

/// @file light_manager.hpp
/// @brief Defines the XnorCore::LightManager class.

BEGIN_XNOR_CORE

class Renderer;

/// @brief Handles rendering lights and their gizmos
class LightManager
{
private:
    static constexpr Vector2i DirectionalShadowMapSize = { 4096, 4096 };
    static constexpr Vector2i SpotLightShadowMapSize = { 1024, 1024 };
    static constexpr Vector2i PointLightLightShadowMapSize = { 1024, 1024 };

    static constexpr float_t LightThreshold = 30.f;
    static constexpr TextureInternalFormat::TextureInternalFormat ShadowDepthTextureInternalFormat = TextureInternalFormat::DepthComponent32F;
public:
    XNOR_ENGINE LightManager() = default;

    XNOR_ENGINE ~LightManager();
    
    DEFAULT_COPY_MOVE_OPERATIONS(LightManager)
    
    /// @brief Initializes the light manager resources
    XNOR_ENGINE void InitResources();

    /// @brief Computes the internal lights to send to the GPU
    /// @param scene Concerned scene
    /// @param renderer Concerned renderer
    XNOR_ENGINE void BeginFrame(const Scene& scene, const Viewport& viewport, Renderer& renderer);
    
    /// @brief End frame
    /// @param scene Concerned scene
    XNOR_ENGINE void EndFrame(const Scene& scene);

    /// @brief Draws the light gizmos
    /// @param camera Editor camera
    /// @param scene Concerned scene
    XNOR_ENGINE void DrawLightGizmo(const Camera& camera, const Scene& scene) const ;

    /// @brief Draws the light gizmos using a specified shader
    /// @param camera Editor camera
    /// @param scene Concerned scene
    /// @param shader Shader to use
    XNOR_ENGINE void DrawLightGizmoWithShader(const Camera& camera, const Scene& scene, const Pointer<Shader>& shader) const;

    /// @brief Binds the shadow map
    XNOR_ENGINE void BindShadowMap() const;

    /// @brief Binds the shadow map
    XNOR_ENGINE void UnBindShadowMap() const;

private:
    enum class RenderingLight
    {
        PointLight,
        SpothLight,
        DirLight
    };

    struct GizmoLight
    {
        Vector3 pos;
        const Light* light = nullptr;
        RenderingLight type;
    };

    struct RenderingLightStruct
    {
        float_t scaleFactor = 2.f;
        float_t minDistance = 1.f;
        float_t minScalarFactor = 10.f;
        float_t maxScalarFactor = 2.f;
        
        Pointer<Texture> pointLightTexture;
        Pointer<Texture> dirLightTexture;
        Pointer<Texture> spotLightTexture;
        Pointer<Shader> editorUi;
        Pointer<Mesh> quad;
    };
    
    GpuLightData* m_GpuLightData = nullptr;
    RenderingLightStruct m_RenderingLightStruct;
    
    Pointer<Shader> m_ShadowMapShader;
    Pointer<Shader> m_ShadowMapShaderPointLight;

    Pointer<Shader> m_ShadowMapShaderSkinned;
    Pointer<Shader>m_ShadowMapShaderPointLightSkinned;

    
    RenderPass m_ShadowRenderPass;
    Framebuffer* m_ShadowFrameBuffer {nullptr};
    Framebuffer* m_ShadowFrameBufferPointLight {nullptr};

    Texture* m_SpotLightShadowMapTextureArray = nullptr;
    
    Texture* m_PointLightShadowMapCubemapArrayPixelDistance = nullptr;
    Texture* m_DepthBufferForPointLightPass = nullptr;
    Texture* m_DirectionalShadowMaps = nullptr;

    mutable std::vector<const PointLight*> m_PointLights;
    mutable std::vector<const SpotLight*> m_SpotLights;
    mutable std::vector<const DirectionalLight*> m_DirectionalLights;
    
    CascadeShadowMap m_CascadeShadowMap;
    
    
    XNOR_ENGINE void FecthLightInfo() const;

    XNOR_ENGINE void ComputeShadow(const Scene& scene, const Viewport& viewport, Renderer& renderer);

    XNOR_ENGINE void ComputeShadowDirLight(const Scene& scene, const Camera& viewPortCamera, Vector2i viewportSize, Renderer& renderer);

    XNOR_ENGINE void ComputeShadowSpotLight(const Scene& scene, Renderer& renderer);

    XNOR_ENGINE void ComputeShadowPointLight(const Scene& scene, Renderer& renderer);
    
    XNOR_ENGINE void GetDistanceFromCamera(std::map<float_t, GizmoLight>* sortedLight, const Camera& camera) const;

    XNOR_ENGINE void GetPointLightDirection(size_t index, Vector3* front,Vector3* up) const;
    
    XNOR_ENGINE void InitShadowMap();

    XNOR_ENGINE void InitShader();
};

END_XNOR_CORE
