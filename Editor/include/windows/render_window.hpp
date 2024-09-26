#pragma once

#include "definitions.hpp"
#include "ui_window.hpp"
#include "rendering/camera.hpp"
#include "rendering/frame_buffer.hpp"

BEGIN_XNOR_EDITOR

class RenderWindow : public UiWindow
{
public:
    
    explicit RenderWindow(Editor* editor, XnorCore::Viewport& viewport);

    explicit RenderWindow(Editor* editor, const std::string& title, XnorCore::Viewport& viewport);
    
    ~RenderWindow() override = default;
    
    DEFAULT_COPY_MOVE_OPERATIONS(RenderWindow)
    
    void Display() override;

    void OnApplicationRendering() override;

    void FetchInfo() override;
    

    [[nodiscard]]
    XnorCore::Input::BoundWindowInfo GetBindWindowInfo() const override;

protected:
    XnorCore::Viewport* m_Viewport = nullptr;

};

END_XNOR_EDITOR
