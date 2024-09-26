#include "scene/component/static_mesh_renderer.hpp"

using namespace  XnorCore;

void StaticMeshRenderer::GetAabb(Bound* const bound) const
{
    if (mesh.IsValid())
        *bound = Bound::GetAabbFromTransform(mesh->aabb, GetTransform());
}
