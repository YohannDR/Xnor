#pragma once

#include "core.hpp"

#include <ImGui/imgui.h>

#include "definitions.hpp"
#include "editor.hpp"
#include "Maths/vector2i.hpp"

BEGIN_XNOR_EDITOR

class UiWindow
{
public:
    UiWindow(Editor* editor, std::string title);
    
    virtual ~UiWindow() = default;
    
    DEFAULT_COPY_MOVE_OPERATIONS(UiWindow)

    int32_t windowFlags = ImGuiWindowFlags_NoFocusOnAppearing;

    [[nodiscard]]
    const std::string& GetName() const;
    
    virtual void Display() = 0;

    virtual void SetParam([[maybe_unused]] void* const param) {}
    
    [[nodiscard]]
    bool IsFocused() const;
    
    [[nodiscard]]
    Vector2i GetPosition() const;
    
    [[nodiscard]]
    Vector2i GetSize() const;
    
    virtual void FetchInfo();

    // Whenever the window need to render after the update of the world
    virtual void OnApplicationRendering();

    virtual XnorCore::Input::BoundWindowInfo GetBindWindowInfo() const;
    
    bool_t opened = true;
    bool_t canClose = false;

protected:
    std::string m_WindowTitle = "Untitled";
    uint32_t m_InputBindId = 0;
    
    bool_t m_IsFocused;
    Vector2i m_Position;
    Vector2i m_Size;

    Editor* m_Editor;
};

END_XNOR_EDITOR
