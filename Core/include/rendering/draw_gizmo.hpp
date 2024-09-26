#pragma once

#include <vector>

#include <Maths/vector3.hpp>

#include "rendering/render_pass.hpp"
#include "rendering/viewport.hpp"
#include "utils/color.hpp"
#include "utils/pointer.hpp"

BEGIN_XNOR_CORE

class DrawGizmo
{
    struct GizmoSphere
    {
        Vector3 position;
        float_t radius = 1.f;
        Color color;
    };

    struct GizmoRectangle
    {
        Vector3 position;
        Vector3 size;
        Color color;
    };

    struct GizmoCapsule
    {
        Vector3 position;
        float_t height = 1.f;
        float_t radius = 1.f;
        Color color;
    };
    
    XNOR_ENGINE static inline std::vector<GizmoSphere> m_GizmoSphereVector;
    XNOR_ENGINE static inline std::vector<GizmoRectangle> m_GizmoRectangleVector;
    XNOR_ENGINE static inline std::vector<GizmoCapsule> m_GizmoCapsuleVector;
    
public:
    XNOR_ENGINE DrawGizmo();

    XNOR_ENGINE ~DrawGizmo() = default;

    DEFAULT_COPY_MOVE_OPERATIONS(DrawGizmo)
    
    XNOR_ENGINE static void Sphere(Vector3 position, float_t radius = 1.f, Color color = Color::Green());
    
    XNOR_ENGINE static void Rectangle(Vector3 position, Vector3 size, Color color = Color::Green());
    
    XNOR_ENGINE static void Capsule(Vector3 position, float_t height, float_t radius, Color color = Color::Green());

    XNOR_ENGINE void DrawGizmos(const Viewport& viewport, const Entity* selectedEntity);

private:
    Pointer<Shader> m_GizmoShader;

    Pointer<Mesh> m_Sphere;

    Pointer<Mesh> m_Cube;

    Pointer<Mesh> m_Capsule;
    
    RenderPass m_RenderPass;

    ModelUniformData modelData;
    
    XNOR_ENGINE void DrawRectangle();

    XNOR_ENGINE void DrawSphere();
    
    XNOR_ENGINE void Clear();
    
    XNOR_ENGINE void DrawCollider(const Entity& entity);
};

END_XNOR_CORE
