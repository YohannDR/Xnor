// ReSharper disable CppClangTidyCppcoreguidelinesPreferMemberInitializer
#include "windows/animation_window.hpp"

#include "reflection/type_renderer.hpp"
#include "resource/animation_montage.hpp"
#include "resource/resource_manager.hpp"
#include "windows/render_window.hpp"

using namespace XnorEditor;

AnimationWindow::AnimationWindow(Editor* editor)
    : RenderWindow(editor, "AnimationEditor", m_Viewport), m_EditorCamera(*editor, m_Camera)
{
    m_Viewport.camera = &m_Camera;
    m_Scene.CreateEntity("Montage");
    XnorCore::Entity* const entity = m_Scene.GetEntities()[0];

    entity->AddComponent(m_SkinnedRenderer);

    m_Viewport.Init(XnorCore::Window::GetSize());
}

void AnimationWindow::Display()
{
    // XnorCore::Filters::FilterResources<XnorCore::Animation>(&m_Animation);
    
    if (m_Animation == nullptr)
    {
        ImGui::Text("No montage selected");
        return;
    }

    m_EditorCamera.UpdateCamera();
    m_SkinnedRenderer->Update();

    XnorCore::TypeRenderer::DisplayObject(m_Animation);

    ImGui::Image(XnorCore::Utils::IntToPointer<ImTextureID>(m_Viewport.image->GetId()), ImGui::GetContentRegionAvail(),  ImVec2(0, 1), ImVec2(1, 0));
}

void AnimationWindow::OnApplicationRendering()
{
    RenderWindow::OnApplicationRendering();
}

void AnimationWindow::SetParam(void* const param)
{
    m_Animation = static_cast<decltype(m_Animation)>(param);

    /*m_SkinnedRenderer->mesh = m_Animation->skeleton->mesh;
    m_SkinnedRenderer->StartAnimation(m_Animation);*/
}
