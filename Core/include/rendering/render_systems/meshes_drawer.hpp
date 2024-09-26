#pragma once
#include "core.hpp"
#include "rendering/frustum.hpp"

#include "scene/scene.hpp"
#include "scene/component/skinned_mesh_renderer.hpp"


BEGIN_XNOR_CORE
class Renderer;

class MeshesDrawer
{
public:
    XNOR_ENGINE MeshesDrawer();

    XNOR_ENGINE ~MeshesDrawer();

    DEFAULT_COPY_MOVE_OPERATIONS(MeshesDrawer)

    XNOR_ENGINE void InitResources();
    void DrawAabb(const Pointer<Mesh> cube) const;

    XNOR_ENGINE void BeginFrame(const Scene& scene, const Renderer& renderer);
    
    XNOR_ENGINE void EndFrame();
    
    // Render All the animated mesh only work on Deferred Rendering
    XNOR_ENGINE void RenderAnimation() const;

    XNOR_ENGINE void RenderAnimationNonShaded(const Scene& scene) const;

    XNOR_ENGINE void RenderStaticMesh(const MaterialType material,const Camera& camera, const Frustum& frustum, const Scene& scene) const;

    XNOR_ENGINE void RenderStaticMeshNonShaded(const Camera& camera, const Frustum& frustum, const Scene& scene) const;



private:
    SkinnedMeshGpuData* m_SkinnedMeshGpuData = nullptr;

    Pointer<Shader> m_SkinnedShader;

    Pointer<Shader> m_GizmoShader;
    
    std::vector<const SkinnedMeshRenderer*> m_SkinnedRender;

    std::vector<const StaticMeshRenderer*> m_StaticMeshs;
    


    XNOR_ENGINE void PrepareOctree(const Scene& scene);
    
};

END_XNOR_CORE