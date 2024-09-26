#pragma once

#include "definitions.hpp"
#include "editor_camera.hpp"
#include "render_window.hpp"
#include "windows/ui_window.hpp"
#include "windows/render_window.hpp"

BEGIN_XNOR_CORE

class AnimationMontage;

END_XNOR_CORE

BEGIN_XNOR_EDITOR

class AnimationMontageWindow final : public RenderWindow
{
public:
    using RenderWindow::RenderWindow;

    explicit AnimationMontageWindow(Editor* editor);

    void Display() override;

    void SetParam(void* param) override;

private:
    XnorCore::AnimationMontage* m_Montage = nullptr;
    XnorCore::SkinnedMeshRenderer* m_SkinnedRenderer = new XnorCore::SkinnedMeshRenderer();

    XnorCore::Viewport m_Viewport;
    EditorCamera m_EditorCamera;
    XnorCore::Camera m_Camera;
    XnorCore::Scene m_Scene;
};

END_XNOR_EDITOR
