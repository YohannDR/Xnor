// ReSharper disable CppClangTidyCppcoreguidelinesPreferMemberInitializer
#include "windows/animation_montage_window.hpp"
#include "resource/animation_montage.hpp"
#include "resource/resource_manager.hpp"

using namespace XnorEditor;

AnimationMontageWindow::AnimationMontageWindow(Editor* editor)
    : RenderWindow(editor,"AnimationMontageEditor",m_Viewport), m_EditorCamera(*editor, m_Camera)
{
    m_Viewport.camera = &m_Camera;
    m_Scene.CreateEntity("Montage");
    XnorCore::Entity* const entity = m_Scene.GetEntities()[0];

    entity->AddComponent(m_SkinnedRenderer);

    m_Viewport.Init(XnorCore::Window::GetSize());
}

void AnimationMontageWindow::Display()
{
    if (m_Montage == nullptr)
    {
        ImGui::Text("No montage selected");
        return;
    }

    m_EditorCamera.UpdateCamera();
    m_SkinnedRenderer->Update();

    ImGui::Image(XnorCore::Utils::IntToPointer<ImTextureID>(m_Viewport.image->GetId()), ImGui::GetContentRegionAvail(),  ImVec2(0, 1), ImVec2(1, 0));
}



void AnimationMontageWindow::SetParam(void* const param)
{
    m_Montage = static_cast<decltype(m_Montage)>(param);

    m_Montage->mesh = XnorCore::ResourceManager::Get<XnorCore::Mesh>("assets/models/Coyote-Running.fbx");

    m_Montage->AddAnimation(0.f, size_t{0});
    m_Montage->AddAnimation(1.2f, XnorCore::ResourceManager::Get<XnorCore::Mesh>("assets/models/Coyote-Attack3.fbx")->GetAnimation(0));
    m_Montage->AddAnimation(3.f, size_t{0});
    m_Montage->AddAnimation(4.2f, XnorCore::ResourceManager::Get<XnorCore::Mesh>("assets/models/Coyote-Walking.fbx")->GetAnimation(0));

    m_SkinnedRenderer->mesh = m_Montage->mesh;
    m_SkinnedRenderer->StartMontage(m_Montage);

    m_Montage->looping = true;
}
