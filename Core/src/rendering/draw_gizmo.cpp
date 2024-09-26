#include "rendering/draw_gizmo.hpp"

#include "physics/component/box_collider.hpp"
#include "physics/component/capsule_collider.hpp"
#include "physics/component/collider.hpp"
#include "physics/component/sphere_collider.hpp"
#include "rendering/rhi.hpp"
#include "resource/resource_manager.hpp"

using namespace XnorCore;

DrawGizmo::DrawGizmo()
{
     m_Sphere = ResourceManager::Get<Model>("assets/models/sphere.obj");
     m_Cube = ResourceManager::Get<Model>("assets/models/cube.obj");
     m_Capsule = ResourceManager::Get<Model>("assets/models/capsule.obj");
     m_GizmoShader = ResourceManager::Get<Shader>("gizmo_shader");
}

void DrawGizmo::Sphere(const Vector3 position, const float_t radius, const Color color)
{
    m_GizmoSphereVector.push_back({ position, radius, color });
}

void DrawGizmo::Rectangle(const Vector3 position, const Vector3 size, const Color color)
{
    m_GizmoRectangleVector.push_back({ position, size, color });
}

void DrawGizmo::Capsule(const Vector3 position, const float_t height, const float_t radius, const Color color)
{
    m_GizmoCapsuleVector.push_back({ position, height, radius, color });
}

void DrawGizmo::DrawGizmos(const Viewport& viewport, const Entity* selectedEntity)
{
    const RenderPassBeginInfo renderPassBeginInfo =
    {
        .frameBuffer = viewport.frameBuffer,
        .renderAreaOffset = { 0, 0 },
        .renderAreaExtent = viewport.viewPortSize,
        .clearBufferFlags = BufferFlag::None,
        .clearColor = Vector4::Zero()
    };
    m_RenderPass.BeginRenderPass(renderPassBeginInfo);
    m_GizmoShader->Use();
    Rhi::SetPolygonMode(PolygonFace::FrontAndBack, PolygonMode::Line);

    if (selectedEntity != nullptr)
        DrawCollider(*selectedEntity);

    DrawRectangle();
    DrawSphere();
    
    m_RenderPass.EndRenderPass();
    Rhi::SetPolygonMode(PolygonFace::FrontAndBack, PolygonMode::Fill);
    m_GizmoShader->Unuse();
    
    Clear();
}

void DrawGizmo::DrawCollider(const Entity& entity)
{
    std::vector<const Collider*> entityCollider;
    entity.GetComponents<Collider>(&entityCollider);
    
    if (entityCollider.empty())
        return;

    m_GizmoShader->SetVec3("color", static_cast<Vector3>(Color::Green()));

    for (const Collider* coll : entityCollider)
    {
        Pointer<Mesh> mesh;
        Matrix colliderMatrix;

        const Vector3 worldPos = static_cast<Vector3>(entity.transform.worldMatrix[3]) + coll->center;

        if (Utils::GetTypeHash<Collider>(coll) == Utils::GetTypeHash<SphereCollider>())
        {
            mesh = m_Sphere;
            const float_t radius = reinterpret_cast<const SphereCollider*>(coll)->radius;
            colliderMatrix = Matrix::Trs(worldPos, Quaternion::Identity(), Vector3(radius));
        }
        else if (Utils::GetTypeHash<Collider>(coll) == Utils::GetTypeHash<BoxCollider>())
        {
            mesh = m_Cube;
            const BoxCollider* boxCollider = reinterpret_cast<const BoxCollider*>(coll);
            colliderMatrix = entity.transform.worldMatrix * Matrix::Trs(Vector3::Zero(), Quaternion::Identity(), boxCollider->size);
        }
        else if (Utils::GetTypeHash<Collider>(coll) == Utils::GetTypeHash<CapsuleCollider>())
        {
            mesh = m_Capsule;
            const CapsuleCollider* capsuleCollider = reinterpret_cast<const CapsuleCollider*>(coll);
            const Vector3 capsuleScale = Vector3(capsuleCollider->radius, capsuleCollider->height, capsuleCollider->radius);
            colliderMatrix = Matrix::Trs(worldPos, Quaternion::Identity(), capsuleScale);
        }
        else
        {
            return;
        }
        modelData.model = colliderMatrix;
        Rhi::UpdateModelUniform(modelData);
        Rhi::DrawModel(DrawMode::Triangles, mesh->models[0]->GetId());
    }
    
    
    
}

void DrawGizmo::DrawRectangle()
{
    for (GizmoRectangle& gizmo : m_GizmoRectangleVector)
    {
        m_GizmoShader->SetVec3("color", static_cast<Vector3>(gizmo.color));
        modelData.model = Matrix::Trs(gizmo.position, Quaternion::Identity(), Vector3(gizmo.size));
        Rhi::UpdateModelUniform(modelData);
        Rhi::DrawModel(DrawMode::Triangles, m_Cube->models[0]->GetId());
    }
}

void DrawGizmo::DrawSphere()
{
    for (GizmoSphere& gizmo : m_GizmoSphereVector)
    {
        m_GizmoShader->SetVec3("color", static_cast<Vector3>(gizmo.color));
        modelData.model = Matrix::Trs(gizmo.position, Quaternion::Identity(), Vector3(gizmo.radius));
        Rhi::UpdateModelUniform(modelData);
        Rhi::DrawModel(DrawMode::Triangles, m_Sphere->models[0]->GetId());
    }
}

void DrawGizmo::Clear()
{
    m_GizmoSphereVector.clear();
    m_GizmoRectangleVector.clear();
    m_GizmoCapsuleVector.clear();
}
