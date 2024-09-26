#pragma once

#include <Maths/vector4.hpp>

#include "core.hpp"
#include "frustum.hpp"
#include "material.hpp"
#include "viewport.hpp"
#include "render_systems/gui_pass.hpp"
#include "render_systems/light_manager.hpp"
#include "render_systems/meshes_drawer.hpp"
#include "render_systems/skybox_renderer.hpp"
#include "render_systems/post_process_pass.hpp"

/// @file renderer.hpp
/// @brief Defines the XnorCore::Renderer class

BEGIN_XNOR_CORE

class DirectionalLight;
class SpotLight;
class PointLight;
class StaticMeshRenderer;

/// @brief Handles rendering a scene given a RendererContext
class Renderer
{
public:
    /// @brief Clear color
    Vector4 clearColor = Vector4(0.f);
    
    LightManager lightManager;
    
    SkyboxRenderer skyboxRenderer;
    
    PostProcessPass postProcessPass;
    
    MeshesDrawer meshesDrawer;
    
    GuiPass guiPass;

    XNOR_ENGINE static inline bool_t IsCsm = false;
    
    XNOR_ENGINE Renderer() = default;
    XNOR_ENGINE ~Renderer() = default;

    DEFAULT_COPY_MOVE_OPERATIONS(Renderer)

    /// @brief Initializes the renderer
    XNOR_ENGINE void Initialize();

    /// @brief Renders a scene on a viewport
    /// @param viewport Viewport target
    /// @param scene Scene to render
    XNOR_ENGINE void RenderViewport(const Viewport& viewport, const Scene& scene);

    /// @brief Renders a scene without shading , calling begin and endfrane 
    /// @param camera Camera
    /// @param renderPassBeginInfo Render pass begin info
    /// @param renderPass Render pass
    /// @param shaderToUseStatic Shader to use
    /// @param scene Scene to render
    /// @param drawEditorUi Whether to draw the editor only UI
    XNOR_ENGINE void ZPass(const Scene& scene, const Camera& camera, const RenderPassBeginInfo& renderPassBeginInfo, const RenderPass& renderPass,
                           const Pointer<Shader>& shaderToUseStatic, const Pointer<Shader> shaderToUseSkinned, bool_t drawEditorUi);

    /// @brief Renders a scene without shading
    /// @param cameraData Camera
    /// @param renderPassBeginInfo Render pass begin info
    /// @param renderPass Render pass
    /// @param shaderToUseStatic Shader to use
    /// @param scene Scene to render
    /// @param drawEditorUi Whether to draw the editor only UI
    XNOR_ENGINE void RenderNonShadedPass(const Scene& scene, const Camera& cameraData, const RenderPassBeginInfo& renderPassBeginInfo, const RenderPass& renderPass,
                                         const Pointer<Shader>& shaderToUseStatic, const Pointer<Shader>& shaderToUseSkinned, bool_t drawEditorUi);

    /// @brief Renders a scene without shading
    /// @param cameraData Camera
    /// @param renderPassBeginInfo Render pass begin info
    /// @param renderPass Render pass
    /// @param shaderToUseStatic Shader to use
    /// @param scene Scene to render
    /// @param drawEditorUi Whether to draw the editor only UI
    XNOR_ENGINE void RenderNonShadedPass(const Scene& scene, const CameraUniformData& cameraData, const RenderPassBeginInfo& renderPassBeginInfo, const RenderPass& renderPass,
                                         const Pointer<Shader>& shaderToUseStatic, const Pointer<Shader>& shaderToUseSkinned, bool_t drawEditorUi);


    /// @brief Swaps the front and back buffer.
    XNOR_ENGINE void SwapBuffers() const;
    
    XNOR_ENGINE static inline bool_t isCsm = false;

private:
     mutable Frustum m_Frustum;
    
     Pointer<Shader> m_GBufferShader;
     Pointer<Shader> m_GBufferShaderLit;
    
     Pointer<Shader> m_Forward;
     Pointer<Shader> m_DrawTextureToScreenShader;
     Pointer<Shader> m_GizmoShader;
    
     Pointer<Mesh> m_Quad;
     Pointer<Mesh> m_Cube;
    
    /// @brief Update All the infos of the current frame lights, animation, etc...
    /// @param scene The scene
    XNOR_ENGINE void BeginFrame(const Scene& scene, const Viewport& viewport);

    /// @brief stuff made at the end of the frame
    /// @param scene The scene
    XNOR_ENGINE void EndFrame(const Scene& scene);
    
    
    XNOR_ENGINE void BindCamera(const Camera& camera, const Vector2i screenSize) const;
    
    XNOR_ENGINE void InitResources();
    
    XNOR_ENGINE void DrawMeshRendersByType(MaterialType materialType , const Scene& scene) const;
    

    XNOR_ENGINE void DeferredRendering(const Camera& camera, const Scene& scene, const ViewportData& viewportData, const Vector2i viewportSize) const;
    
    XNOR_ENGINE void ForwardPass(const Scene& scene,
        const Viewport& viewport, Vector2i viewportSize, bool_t isEditor) const;
    
};

END_XNOR_CORE
