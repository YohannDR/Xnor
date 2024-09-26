#include "windows/header_window.hpp"

#include <ImGui/imgui.h>

#include "csharp/dotnet_runtime.hpp"
#include "file/file_manager.hpp"
#include "resource/resource_manager.hpp"
#include "scene/scene.hpp"
#include "serialization/serializer.hpp"
#include "utils/coroutine.hpp"
#include "utils/utils.hpp"
#include "world/world.hpp"

using namespace XnorEditor;

HeaderWindow::HeaderWindow(Editor* editor)
    : UiWindow(editor, "Header")
{
    windowFlags |= ImGuiWindowFlags_NoScrollbar;
    
    m_PlayButton = XnorCore::ResourceManager::Get<XnorCore::Texture>("assets_internal/editor/ui/header/play_button.png");
    m_PauseButton = XnorCore::ResourceManager::Get<XnorCore::Texture>("assets_internal/editor/ui/header/pause_button.png");
    m_ResumeButton = XnorCore::ResourceManager::Get<XnorCore::Texture>("assets_internal/editor/ui/header/resume_button.png");
    m_StopButton = XnorCore::ResourceManager::Get<XnorCore::Texture>("assets_internal/editor/ui/header/stop_button.png");
}

void HeaderWindow::Display()
{
    m_ImageSize = TileBaseSize;
    
    for (size_t i = 0; i < ImageCount; i++)
    {
        constexpr float_t constantOffsetValue = 1.5f;
        const float_t offSet = m_ImageSize * static_cast<float_t>(ImageCount - (i + 1)) * constantOffsetValue;
        const float_t x = (static_cast<float_t>(m_Size.x) - m_ImageSize) * 0.5f;
        const float_t y = (static_cast<float_t>(m_Size.y) - m_ImageSize) * 0.5f;

        m_ImagePos[i].x = x - offSet;
        m_ImagePos[i].y = y;
    }

    if (m_Editor->IsGamePlaying())
        DisplayOnPlay();
    else
        DisplayOnEditor();
}

void HeaderWindow::DisplayOnEditor()
{
    const ImVec2 currentImagePos = { m_ImagePos[0].x,m_ImagePos[0].y };
    ImGui::SetCursorPos(currentImagePos);

    const bool_t disabled = !XnorCore::World::scene || m_Editor->IsReloadingScripts() || m_Editor->IsDeserializing();

    if (disabled)
        ImGui::BeginDisabled();

    if (ImGui::ImageButton(XnorCore::Utils::IntToPointer<ImTextureID>(m_PlayButton->GetId()), { m_ImageSize, m_ImageSize }))
        m_Editor->StartPlaying();

    if (disabled)
        ImGui::EndDisabled();
}

void HeaderWindow::DisplayOnPlay()
{
    ImVec2 currentImagePos = { m_ImagePos[0].x, m_ImagePos[0].y };
    ImGui::SetCursorPos(currentImagePos);
    
    if (ImGui::ImageButton(XnorCore::Utils::IntToPointer<ImTextureID>(m_StopButton->GetId()), { m_ImageSize, m_ImageSize }))
    {
        m_Editor->StopPlaying();
        return;
    }

    currentImagePos = { m_ImagePos[1].x, m_ImagePos[1].y };

    ImGui::SetCursorPos(currentImagePos);

    if (XnorCore::World::isPlaying)
    {
        if (ImGui::ImageButton(XnorCore::Utils::IntToPointer<ImTextureID>(m_PauseButton->GetId()), { m_ImageSize, m_ImageSize }))
            XnorCore::World::isPlaying = false;
    }
    else
    {
        if (ImGui::ImageButton(XnorCore::Utils::IntToPointer<ImTextureID>(m_ResumeButton->GetId()), { m_ImageSize, m_ImageSize }))
            XnorCore::World::isPlaying = true;
    }
}
