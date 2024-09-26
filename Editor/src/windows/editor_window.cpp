#include "windows/editor_window.hpp"

#include "assimp/Logger.hpp"
#include "utils/logger.hpp"
#include "utils/utils.hpp"
#include "world/world.hpp"

using namespace XnorEditor;

void EditorWindow::OnApplicationRendering()
{
    RenderWindow::OnApplicationRendering();
    m_DrawGizmo.DrawGizmos(*m_Viewport, m_Editor->data.selectedEntity);
}

EditorWindow::EditorWindow(Editor* editor, XnorCore::Viewport& viewport)
    : RenderWindow(editor, "Editor", viewport) , m_PickingStrategy(m_Editor)
  , m_EditorCamera(*m_Editor, m_Editor->data.editorCam)
{
}

void EditorWindow::Display()
{
    RenderWindow::Display();
    m_PickingStrategy.ResizeHandle(m_Size);
    
    m_TransformGizmo.SetRendering(
        m_Editor->data.editorCam,
        {static_cast<float_t>(m_Position.x), static_cast<float_t>(m_Position.y)},
        m_Viewport->viewPortSize
    );

    if (DrawOnTopOfImage())
        return;

    const bool_t isEditingTransform = EditTransform(); 
    
    if (IsFocused())
    {
        m_EditorCamera.UpdateCamera();

        if (!isEditingTransform)
            SelectEntityOnScreen();
    }


    m_EditorCamera.OnPressGoToObject();
}

bool EditorWindow::DrawOnTopOfImage()
{
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.f);
    ImGui::SetCursorPos({25, 25});
    if (ImGui::Button("Translation", {25, 25}))
    {
        m_TransformGizmo.currentGizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
        ImGui::PopStyleVar();
        return true;
    }
    ImGui::PopStyleVar();

    ImGui::SetCursorPos({50, 25});
    if (ImGui::Button("Rotation", {25, 25}))
    {
        m_TransformGizmo.currentGizmoOperation = ImGuizmo::OPERATION::ROTATE;
        return true;
    }

    ImGui::SetCursorPos({75, 25});
    if (ImGui::Button("Scale", {25, 25}))
    {
        m_TransformGizmo.currentGizmoOperation = ImGuizmo::OPERATION::SCALE;
        return true;
    }
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.f);

    ImGui::SetCursorPos({ImGui::GetWindowWidth() - 50, 25});
    if (ImGui::Button("Snap", {25, 25}))
    {
        m_TransformGizmo.useSnap = !m_TransformGizmo.useSnap;
        ImGui::PopStyleVar();
        return true;
    }
    ImGui::PopStyleVar();
    return false;
}

bool EditorWindow::EditTransform()
{
    if (!m_Editor->data.selectedEntity)
        return false;

    return m_TransformGizmo.Manipulate(*m_Editor->data.selectedEntity);
}

void EditorWindow::SelectEntityOnScreen()
{
    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
    {
        const Vector2 mousepos = ImGui::GetMousePos();
        Vector2i mousePosI = {static_cast<int32_t>(mousepos.x), static_cast<int32_t>(mousepos.y)};
        mousePosI -= m_Position;
        // Flip the y axis 
        mousePosI.y = m_Size.y - mousePosI.y;
        if (mousePosI.x >= m_Size.x || mousePosI.y >= m_Size.y || mousePosI.x < 0 || mousePosI.y < 0)
        {
            return;
        }

        XnorCore::Entity* ptr = nullptr;
        if (m_PickingStrategy.GetEntityFromScreen(mousePosI, *XnorCore::World::scene, m_Editor->data.editorCam, &ptr))
        {
            m_Editor->data.selectedEntity = ptr;
            return;
        }

        m_Editor->data.selectedEntity = nullptr;
    }
}
