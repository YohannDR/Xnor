#pragma once

#include "definitions.hpp"
#include "editor_camera.hpp"
#include "ui_window.hpp"
#include "windows/render_window.hpp"

BEGIN_XNOR_EDITOR

class AnimationWindow final : public RenderWindow
{
public:
    using RenderWindow::RenderWindow;

    explicit AnimationWindow(Editor* editor);

    void Display() override;
    void OnApplicationRendering() override;

    void SetParam(void* param) override;

private:
    XnorCore::Animation* m_Animation = nullptr;
    XnorCore::SkinnedMeshRenderer* m_SkinnedRenderer = new XnorCore::SkinnedMeshRenderer;

    XnorCore::Viewport m_Viewport;
    EditorCamera m_EditorCamera;
    XnorCore::Camera m_Camera;
    XnorCore::Scene m_Scene;
};

END_XNOR_EDITOR
