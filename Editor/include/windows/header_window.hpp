#pragma once

#include "definitions.hpp"
#include "ui_window.hpp"
#include "resource/texture.hpp"
#include "utils/pointer.hpp"

BEGIN_XNOR_EDITOR

class HeaderWindow : public UiWindow
{
    static constexpr float_t TileBaseSize = 25.f;
    
public:
    explicit HeaderWindow(Editor* editor);
    
    ~HeaderWindow() override = default;

    DEFAULT_COPY_MOVE_OPERATIONS(HeaderWindow)
    
    void Display() override;

private:
    void DisplayOnEditor();
    void DisplayOnPlay();
    
    // Value to offset the little gray image from imgui
    static constexpr size_t ImageCount = 2;

    std::array<Vector2, ImageCount> m_ImagePos;
    float_t m_ImageSize = 1.f;
    
    XnorCore::Pointer<XnorCore::Texture> m_PlayButton;
    XnorCore::Pointer<XnorCore::Texture> m_PauseButton;
    XnorCore::Pointer<XnorCore::Texture> m_ResumeButton;
    XnorCore::Pointer<XnorCore::Texture> m_StopButton;
};

END_XNOR_EDITOR
