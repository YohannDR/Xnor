#pragma once


#include "core.hpp"
#include "definitions.hpp"
#include "Maths/matrix.hpp"

#include <ImGui/imgui.h>
#include <ImguiGizmo/ImGuizmo.h>

#include "transform.hpp"
#include "rendering/camera.hpp"
#include "scene/entity.hpp"

BEGIN_XNOR_EDITOR

class TransfromGizmo
{
public:
    DEFAULT_COPY_MOVE_OPERATIONS(TransfromGizmo)

    TransfromGizmo() = default;

    ~TransfromGizmo() = default;

    void SetRendering(const XnorCore::Camera& camera, Vector2 windowPos, Vector2i windowSize);

    bool_t Manipulate(XnorCore::Entity& entity);

    bool_t useSnap = false;
    
    Vector3 snap;

    ImGuizmo::OPERATION currentGizmoOperation = ImGuizmo::TRANSLATE;
    
    ImGuizmo::MODE currentGizmoMode = ImGuizmo::WORLD;

private:
    Matrix m_View;
    Matrix m_Projection;

    Vector3 m_SnapTranslation = Vector3(1.f);
    Vector3 m_SnapRotation = Vector3(1.f);
    Vector3 m_SnapScale = Vector3(1.f);

    void UserInput();
};

END_XNOR_EDITOR
