#include "windows/ui_window.hpp"

#include "ImGui/imgui.h"

using namespace XnorEditor;

UiWindow::UiWindow(Editor* const editor, std::string title)
    : m_WindowTitle(std::move(title)) , m_InputBindId(XnorCore::Input::GetBindingId())
    , m_IsFocused(false)
    , m_Editor(editor)
{
}

const std::string& UiWindow::GetName() const
{
    return m_WindowTitle;
}

bool_t UiWindow::IsFocused() const
{
    return m_IsFocused;
}

Vector2i UiWindow::GetPosition() const
{
    return m_Position;
}

Vector2i UiWindow::GetSize() const
{
    return m_Size;
}

void UiWindow::FetchInfo()
{
    m_IsFocused = ImGui::IsWindowFocused();
    if (m_IsFocused)
        XnorCore::Input::BindWindow(m_InputBindId);


    const ImVec2 position = ImGui::GetWindowPos(); 
    m_Position = Vector2i(static_cast<int32_t>(position.x), static_cast<int32_t>(position.y));

    const ImVec2 size = ImGui::GetWindowSize(); 
    m_Size = Vector2i(static_cast<int32_t>(size.x), static_cast<int32_t>(size.y));

}

void UiWindow::OnApplicationRendering()
{
}

XnorCore::Input::BoundWindowInfo UiWindow::GetBindWindowInfo() const
{
    auto& style = ImGui::GetStyle();
    const Vector2 windowPadding = style.WindowPadding * 0.5f;
    const Vector2 framePadding = style.FramePadding * 0.5f;
    constexpr float_t TiletBarOffSet = 15.f; 
    
    Vector2 pos = static_cast<Vector2>(m_Position);
    pos.x += windowPadding.x;
    pos.y += windowPadding.y + framePadding.y + TiletBarOffSet;
    
    const XnorCore::Input::BoundWindowInfo windowInfo =
        {
        .windowPos = pos,
        .windowSize = static_cast<Vector2>(m_Size),
        .isEditor = false,
        .isRendering = false
        };
    
    return windowInfo;
}

