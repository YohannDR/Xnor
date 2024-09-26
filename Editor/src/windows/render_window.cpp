#include "windows/render_window.hpp"

#include "utils/utils.hpp"
#include "world/world.hpp"

using namespace XnorEditor;

void RenderWindow::FetchInfo()
{
    UiWindow::FetchInfo();
    XnorCore::Input::UpdateBoundWindowInfo(m_InputBindId, GetBindWindowInfo());
}

RenderWindow::RenderWindow(Editor* editor, XnorCore::Viewport& viewport)
    : UiWindow(editor, "Game Preview"), m_Viewport(&viewport)
{
    m_Viewport->Init(XnorCore::Window::GetSize());
}

RenderWindow::RenderWindow(Editor* editor, const std::string& title, XnorCore::Viewport& viewport)
    : UiWindow(editor, title) , m_Viewport(&viewport)
{
    m_Viewport->Init(XnorCore::Window::GetSize());
}

void RenderWindow::Display()
{
    ImGui::Image(XnorCore::Utils::IntToPointer<ImTextureID>(m_Viewport->image->GetId()), ImGui::GetContentRegionAvail(), ImVec2(0, 1), ImVec2(1, 0));
}

void RenderWindow::OnApplicationRendering()
{
    if (m_Size == Vector2i::Zero())
        return;
    
    if (m_Viewport->frameBuffer == nullptr || m_Viewport->viewPortSize != m_Size)
    {
        if (m_Viewport->camera)
            m_Viewport->Resize(m_Size);
        
    }

    if (m_Viewport->frameBuffer && m_Viewport->camera != nullptr)
        m_Editor->renderer.RenderViewport(*m_Viewport, *XnorCore::World::scene);
}

XnorCore::Input::BoundWindowInfo RenderWindow::GetBindWindowInfo() const
{
    const ImGuiStyle& style = ImGui::GetStyle();
    const Vector2 windowPadding = style.WindowPadding * 0.5f;
    const Vector2 framePadding = style.FramePadding * 0.5f;
    constexpr float_t tileBarOffSet = 15.f;
    
    Vector2 pos = static_cast<Vector2>(m_Position);
    pos.x += windowPadding.x;
    pos.y += windowPadding.y + framePadding.y + tileBarOffSet;

    const XnorCore::Input::BoundWindowInfo windowInfo =
    {
        .windowPos = pos,
        .windowSize = static_cast<Vector2>(m_Size),
        .isEditor = m_Viewport->isEditor,
        .isRendering = true
    };

    return windowInfo;
}
