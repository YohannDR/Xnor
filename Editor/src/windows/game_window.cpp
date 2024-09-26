#include "windows/game_window.hpp"

using namespace XnorEditor;

void GameWindow::FetchInfo()
{
    RenderWindow::FetchInfo();

    // Only get the input when the window is focused
    XnorCore::Input::LockInput(!m_IsFocused);

    // If the Escape key is pressed
    if (ImGui::IsKeyPressed(ImGuiKey_Escape))
    {
        // If the window is focused, remove the focus
        if (m_IsFocused)
            ImGui::SetWindowFocus(nullptr);
        else
            m_Editor->StopPlaying();
    }
}
