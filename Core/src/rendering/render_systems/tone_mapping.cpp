#include "rendering/render_systems/tone_mapping.hpp"

#include "rendering/frame_buffer.hpp"
#include "rendering/render_pass.hpp"
#include "rendering/rhi.hpp"
#include "resource/resource_manager.hpp"

using namespace XnorCore;

void ToneMapping::ComputeToneMaping(const Texture& imageWithoutToneMapping, const Texture& bloomTexture) const
{
    Rhi::SetViewport({ 0, 0 }, imageWithoutToneMapping.GetSize());
    m_Aces->Use();
    imageWithoutToneMapping.BindTexture(ToneMapedTextureBinding);
    bloomTexture.BindTexture(1);
    Rhi::DrawModel(DrawMode::Triangles, m_Quad->models[0]->GetId());
    m_Aces->Unuse();
}

void ToneMapping::InitializeResources()
{
    m_Aces = ResourceManager::Get<Shader>("tone_mapper");
    m_Aces->CreateInInterface();
    m_Aces->Use();
    m_Aces->SetInt("beforeToneMappedImage", ToneMapedTextureBinding);
    m_Aces->SetInt("bloomBlur", 1);
    m_Aces->Unuse();

    m_Quad = ResourceManager::Get<Model>("assets/models/quad.obj");
}

void ToneMapping::RecompileShader()
{
    m_Aces->Recompile();
}
