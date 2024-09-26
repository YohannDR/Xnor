#include "rendering/render_systems/bloom_pass.hpp"

#include "rendering/rhi.hpp"
#include "resource/resource_manager.hpp"

using namespace XnorCore;

void BloomPass::Init()
{
    m_Quad = ResourceManager::Get<Model>("assets/models/quad.obj");
    // Downsample
    m_DownSample = ResourceManager::Get<ComputeShader>("down_sample");
    m_DownSample->CreateInInterface();
    m_DownSample->Use();
    m_DownSample->SetInt("currentMip", 0);
    m_DownSample->SetInt("nextMip", 1);
    m_DownSample->Unuse();

    // Upsample
    m_UpSample = ResourceManager::Get<ComputeShader>("up_sample");
    m_UpSample->CreateInInterface();
    m_UpSample->Use();
    m_UpSample->SetInt("currentMip", 0);
    m_UpSample->SetInt("nextMip", 1);    
    m_UpSample->Unuse();

    m_ThresholdFilter = ResourceManager::Get<ComputeShader>("bloom_threshold");
    m_ThresholdFilter->CreateInInterface();
    m_ThresholdFilter->Use();
    m_ThresholdFilter->SetInt("baseImage", 0);
    m_ThresholdFilter->SetInt("thresholdTexture", 1);
    m_ThresholdFilter->Unuse();
}

void BloomPass::ComputeBloom(const Texture& imageWithoutBloom, const BloomRenderTarget& bloomRenderTarget) const
{
    ThresholdFilter(imageWithoutBloom, bloomRenderTarget);
    DownSampling(bloomRenderTarget);
    UpSampling(bloomRenderTarget);
}

void BloomPass::UpSampling(const BloomRenderTarget& bloomRenderTarget) const
{   
    const std::vector<BloomRenderTarget::BloomMip>& mipchain = bloomRenderTarget.mipChain;
    
    m_UpSample->Use();
    m_UpSample->SetFloat("bloom_intensity", 1.f);

    for (size_t i = mipchain.size() - 1; i > 0; i--)
    {
        const BloomRenderTarget::BloomMip& mip = mipchain[i];
        const BloomRenderTarget::BloomMip& nextMip = mipchain[i - 1];

        const Vector2 mipSize = { std::floor(nextMip.sizef.x), std::floor(nextMip.sizef.y) };
        m_UpSample->SetVec2("uTexelSize", Vector2(1.0f) / mipSize);
        
        // Source
        mip.texture->BindTexture(0);
        // Target
        m_UpSample->BindImage(1, *nextMip.texture, 0, false, 0, ImageAccess::ReadWrite);

        m_UpSample->DispatchCompute(static_cast<uint32_t>(std::ceil(mipSize.x / ComputeShaderDispactValue)), static_cast<uint32_t>(std::ceil(mipSize.y / ComputeShaderDispactValue)), 1);  

        m_UpSample->SetMemoryBarrier(AllBarrierBits);
    }
    
    m_UpSample->Unuse();
}

void BloomPass::DownSampling(const BloomRenderTarget& bloomRenderTarget) const
{
    m_DownSample->Use();
    bloomRenderTarget.thresholdTexture->BindTexture(0);
    
    for (const BloomRenderTarget::BloomMip& bloomMip : bloomRenderTarget.mipChain)
    {
        m_DownSample->BindImage(1, *bloomMip.texture, 0, false, 0, ImageAccess::ReadWrite);
        const Vector2 mipSize = { std::floor(bloomMip.sizef.x), std::floor(bloomMip.sizef.y) };
        m_DownSample->SetVec2("uTexelSize", Vector2(1.0f) / mipSize);
        
        m_DownSample->DispatchCompute(static_cast<uint32_t>(std::ceil(mipSize.x / ComputeShaderDispactValue)), static_cast<uint32_t>(std::ceil(mipSize.y / ComputeShaderDispactValue)), 1);  
        m_DownSample->SetMemoryBarrier(AllBarrierBits);
        
        bloomMip.texture->BindTexture(0);
    }
    
    bloomRenderTarget.thresholdTexture->UnbindTexture(0);
    m_DownSample->Unuse();
}

void BloomPass::ThresholdFilter(const Texture& imageWithoutBloom, const BloomRenderTarget& bloomRenderTarget) const
{
    const Texture& thresholdTexture = *bloomRenderTarget.thresholdTexture;
    const Vector2i viewportSize = imageWithoutBloom.GetSize();
    
    m_ThresholdFilter->Use();
    
    m_UpSample->BindImage(0, imageWithoutBloom, 0, false, 0, ImageAccess::ReadWrite);
    m_UpSample->BindImage(1, thresholdTexture, 0, false, 0, ImageAccess::ReadWrite);

    m_ThresholdFilter->DispatchCompute(static_cast<uint32_t>(std::ceil(static_cast<float_t>(viewportSize.x) / ComputeShaderDispactValue)), static_cast<uint32_t>(std::ceil(static_cast<float_t>(viewportSize.y) / ComputeShaderDispactValue)) ,1);  
    m_ThresholdFilter->SetMemoryBarrier(AllBarrierBits);
    m_ThresholdFilter->Unuse();
}
