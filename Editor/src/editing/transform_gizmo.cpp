#include "editing/transform_gizmo.hpp"

#include "scene/entity.hpp"

using namespace XnorEditor;


void TransfromGizmo::SetRendering(const XnorCore::Camera& camera, const Vector2 windowPos, const Vector2i windowSize)
{
    ImGuizmo::SetOrthographic(false);
    ImGuizmo::SetDrawlist();
    camera.GetProjection(windowSize, &m_Projection);
    camera.GetView(&m_View);
    ImGuizmo::SetRect(windowPos.x, windowPos.y, static_cast<float_t>(windowSize.x), static_cast<float_t>(windowSize.y));
}

bool_t TransfromGizmo::Manipulate(XnorCore::Entity& entity)
{
    UserInput();
    XnorCore::Transform& transform = entity.transform;
    
    ImGuizmo::Manipulate(m_View.Raw(), m_Projection.Raw(), currentGizmoOperation, currentGizmoMode, transform.worldMatrix.Raw(),
        nullptr,useSnap ? snap.Raw() : nullptr);
    
    if (!ImGuizmo::IsOver() || !ImGuizmo::IsUsing())
        return false;
    
    Vector3 position;
    Vector3 eulerRotation;
    Vector3 scale;

    Matrix matrix = transform.worldMatrix;
    if (entity.HasParent())
    {
        matrix = entity.GetParent()->transform.worldMatrix.Inverted() * transform.worldMatrix;
    }
    
    ImGuizmo::DecomposeMatrixToComponents(matrix.Raw(), position.Raw(), eulerRotation.Raw(), scale.Raw());

    transform.SetPosition(position);
    // Convert Imgui gizmoRot
    transform.SetRotationEulerAngle(eulerRotation * Calc::Deg2Rad);
    transform.SetScale(scale);
    
    return true;
    
}

void TransfromGizmo::UserInput()
{
    if (ImGui::IsWindowFocused())
    {
        if (ImGui::IsKeyPressed(ImGuiKey_T))
            currentGizmoOperation = ImGuizmo::TRANSLATE;
        if (ImGui::IsKeyPressed(ImGuiKey_R))
            currentGizmoOperation = ImGuizmo::ROTATE;
        if (ImGui::IsKeyPressed(ImGuiKey_Y)) 
            currentGizmoOperation = ImGuizmo::SCALE;
    }
    
    switch (currentGizmoOperation)  // NOLINT(clang-diagnostic-switch-enum)
    {
        case ImGuizmo::TRANSLATE:
            snap = m_SnapTranslation;
            break;
        
        case ImGuizmo::ROTATE:
            snap = m_SnapRotation;
            break;
        
        case ImGuizmo::SCALE:
            snap = m_SnapScale;
            break;

        default:
            break;
    }
}

