#include "..\..\include\rendering\bloom_render_target.hpp"

#include "window.hpp"

using namespace XnorCore;

BloomRenderTarget::BloomRenderTarget()
{
    Init(Window::GetSize());
}

BloomRenderTarget::~BloomRenderTarget()
{
    Destroy();
}

void BloomRenderTarget::Init(const Vector2i viewportSize)
{
    CreateBloomMip(viewportSize);
    thresholdTexture = new Texture(TextureInternalFormat::Rgba32F, viewportSize);
    frameBuffer = new Framebuffer();
}

void BloomRenderTarget::Destroy()
{
    for (const BloomMip& mip : mipChain)
        delete mip.texture;

    mipChain.clear();

    delete thresholdTexture;
    delete frameBuffer;
}

void BloomRenderTarget::Resize(const Vector2i viewportSize)
{
    Destroy();
    Init(viewportSize);
}

Texture* BloomRenderTarget::GetBloomedTexture() const
{
    return mipChain[0].texture;
}

void BloomRenderTarget::CreateBloomMip(const Vector2i viewportSize)
{
    for (const BloomMip& mip : mipChain)
        delete mip.texture;
    
    mipChain.resize(BloomMipNumber);
    
    Vector2 mimSizeF = { static_cast<float_t>(viewportSize.x), static_cast<float_t>(viewportSize.y) };
    Vector2i mimSize = { static_cast<int32_t>(viewportSize.x), static_cast<int32_t>(viewportSize.y) };

    for (BloomMip& mip : mipChain)
    {
        mimSizeF *= 0.5f;
        mimSize.x /= 2;
        mimSize.y /= 2;
        mip.sizef = mimSizeF;
        
        const TextureCreateInfo createInfo =
        {
            .size = { static_cast<int32_t>(mimSizeF.x), static_cast<int32_t>(mimSizeF.y) },
            .filtering = TextureFiltering::Linear,
            .wrapping = TextureWrapping::ClampToEdge,
            .format = TextureFormat::Rgba,
            .internalFormat = BloomTextureFormat,
            .dataType = DataType::Float
        };

        mip.texture = new Texture(createInfo);
    }
}
