#include "rendering/render_systems/post_process_pass.hpp"

#include "..\..\..\include\rendering\bloom_render_target.hpp"
#include "..\..\..\include\rendering\post_process_render_target.hpp"
#include "rendering/rhi.hpp"
#include "resource/resource_manager.hpp"

using namespace XnorCore;

PostProcessPass::~PostProcessPass()
{
    delete m_FrameBuffer;
    m_FrameBuffer = nullptr;
}

void PostProcessPass::Init()
{
    m_ToneMapping.InitializeResources();
    m_BloomPass.Init();
    m_FrameBuffer = new Framebuffer();
}

void PostProcessPass::Compute(const Texture& textureToCompute, const Texture& postProcessedTexture, const PostProcessRenderTarget& postProcessRendertarget) const
{
    m_BloomPass.ComputeBloom(textureToCompute,postProcessRendertarget.bloomRenderTarget);

    
    m_FrameBuffer->AttachTexture(postProcessedTexture, Attachment::Color00, 0);
    const RenderPassBeginInfo beginInfo =
    {
        .frameBuffer = m_FrameBuffer,
        .renderAreaOffset = { 0, 0 },
        .renderAreaExtent = textureToCompute.GetSize(),
        .clearBufferFlags = BufferFlag::None,
        .clearColor = Vector4::Zero()
    };
    m_RenderPass.BeginRenderPass(beginInfo);
    m_ToneMapping.ComputeToneMaping(textureToCompute,*postProcessRendertarget.bloomRenderTarget.GetBloomedTexture());
    m_RenderPass.EndRenderPass();
}
