#pragma once

#include "editor_camera.hpp"
#include "render_window.hpp"
#include "editing/picking_strategy.hpp"
#include "editing/transform_gizmo.hpp"
#include "rendering/draw_gizmo.hpp"

BEGIN_XNOR_EDITOR

class EditorWindow : public RenderWindow
{
public:
    void OnApplicationRendering() override;

    explicit EditorWindow(Editor* editor, XnorCore::Viewport& viewport);

    ~EditorWindow() override = default;

    DEFAULT_COPY_MOVE_OPERATIONS(EditorWindow)

    void Display() override;
    
protected:
    bool DrawOnTopOfImage();

    bool EditTransform();

    void SelectEntityOnScreen();

    PickingStrategy m_PickingStrategy;

private:
    EditorCamera m_EditorCamera;
    TransfromGizmo m_TransformGizmo;
    XnorCore::DrawGizmo m_DrawGizmo;
};

END_XNOR_EDITOR
