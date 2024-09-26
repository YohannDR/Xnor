#include "..\..\..\include\rendering\render_systems\skybox_parser.hpp"

#include "rendering/frame_buffer.hpp"
#include "rendering/rhi.hpp"
#include "resource/resource_manager.hpp"
#include "utils/logger.hpp"

using namespace XnorCore;

SkyBoxParser::~SkyBoxParser()
{
    delete m_FrameBuffer;
    delete m_TextureDepth;
}

void SkyBoxParser::Init()
{
    m_Cube = ResourceManager::Get<Model>("assets/models/cube.obj");
    m_Quad = ResourceManager::Get<Model>("assets/models/quad.obj");
    
    m_EquirectangularToCubeMapShader = ResourceManager::Get<Shader>("equirectangular_to_cubemap");
    m_EquirectangularToCubeMapShader->SetDepthFunction(DepthFunction::LessEqual);
    m_EquirectangularToCubeMapShader->CreateInInterface();
    m_EquirectangularToCubeMapShader->Use();
    m_EquirectangularToCubeMapShader->SetInt("equirectangularMap",0);
    m_EquirectangularToCubeMapShader->Use();

    m_IrradianceConvolution = ResourceManager::Get<Shader>("irradiance_convolution");
    m_IrradianceConvolution->SetDepthFunction(DepthFunction::LessEqual);
    m_IrradianceConvolution->CreateInInterface();
    
    m_IrradianceConvolution->Use();
    m_IrradianceConvolution->SetInt("environmentMap", 0);
    m_IrradianceConvolution->Unuse();

    m_PrefilterShader = ResourceManager::Get<Shader>("prefilter_shader");
    m_PrefilterShader->SetDepthFunction(DepthFunction::LessEqual);
    m_PrefilterShader->CreateInInterface();
    m_PrefilterShader->Use();
    m_PrefilterShader->SetInt("environmentMap",0);
    m_PrefilterShader->Use();

    m_PreComputeBrdr = ResourceManager::Get<Shader>("precompute_brdf");
    m_PreComputeBrdr->CreateInInterface();

    m_FrameBuffer = new Framebuffer();
}

void SkyBoxParser::EquirectangularToCubeMapFunc(const Texture& equirectangularMap, const Texture& cubemap)
{
    Compute(equirectangularMap, cubemap, m_EquirectangularToCubeMapShader);
}

void SkyBoxParser::ComputeIrradiance(const Texture& irradianceInput, const Texture& irradianceOutput, const Vector2i irradianceSize) 
{
    Matrix projection;
    Matrix::Perspective(90.f * Calc::Deg2Rad, 1.0f, 0.1f, 10.f, &projection);
    std::array<Matrix, static_cast<size_t>(CubeMapFace::Size)> captureViews;
    Rhi::GetCubeMapViewMatrices(&captureViews);

    m_IrradianceConvolution->Use();
    m_IrradianceConvolution->SetMat4("projection", projection);
    irradianceInput.BindTexture(0);
    
    for (size_t i = 0; i < 6; i++)
    {
        const RenderPassBeginInfo renderPassBeginInfo =
        {
            .frameBuffer = m_FrameBuffer,
            .renderAreaOffset = { 0, 0 },
            .renderAreaExtent = irradianceSize,
            .clearBufferFlags = static_cast<decltype(renderPassBeginInfo.clearBufferFlags)>(BufferFlag::ColorBit | BufferFlag::DepthBit)
        };

        m_IrradianceConvolution->SetMat4("view", captureViews[i]);

        m_FrameBuffer->AttachTextureLayer(irradianceOutput, Attachment::Color00, 0, static_cast<uint32_t>(i));

        m_RenderPass.BeginRenderPass(renderPassBeginInfo);

        Rhi::DrawModel(DrawMode::Triangles, m_Cube->models[0]->GetId());

        m_RenderPass.EndRenderPass();
    }
}

void SkyBoxParser::ComputePreFiltering(const Texture& environementMap, const Texture& prefilteringMap, const uint32_t mipLevels)
{
    Matrix projection;
    Matrix::Perspective(90.f * Calc::Deg2Rad, 1.0f, 0.1f, 10.f,&projection);
    std::array<Matrix, static_cast<size_t>(CubeMapFace::Size)> captureViews;
    Rhi::GetCubeMapViewMatrices(&captureViews);
    
    m_PrefilterShader->Use();
    environementMap.BindTexture(0);
    m_PrefilterShader->SetMat4("projection", projection);

    for (uint32_t mip = 0; mip < mipLevels; mip++)
    {
        const uint32_t mipWidth = static_cast<uint32_t>(128 * std::pow(0.5, mip));
        const uint32_t mipHeight = static_cast<uint32_t>(128 * std::pow(0.5, mip));
        
        const float_t roughness = static_cast<float_t>(mip) / static_cast<float_t>(mipLevels - 1);
        m_PrefilterShader->SetFloat("roughness", roughness);
        RenderPassBeginInfo renderPassBeginInfo =
        {
            .frameBuffer = m_FrameBuffer,
            .renderAreaOffset = { 0, 0 },
            .renderAreaExtent = { static_cast<int32_t>(mipWidth), static_cast<int32_t>(mipHeight) }
        };
        
        m_RenderPass.BeginRenderPass(renderPassBeginInfo);
        
        for (uint32_t i = 0; i < 6; i++)
        {
            m_FrameBuffer->AttachTextureLayer(prefilteringMap, Attachment::Color00, mip, i);

            Rhi::ClearBuffer(BufferFlag::DepthBit);
            m_PrefilterShader->SetMat4("view", captureViews[i]);
            
            Rhi::DrawModel(DrawMode::Triangles, m_Cube->models[0]->GetId());
        }
    }
    m_RenderPass.EndRenderPass();

    m_PrefilterShader->Unuse();
}

void SkyBoxParser::PreComputeBrdf(const Vector2i environementMapSize, const Texture& brdfTexture)
{
    RenderPassBeginInfo renderPassBeginInfo =
    {
        .frameBuffer = m_FrameBuffer,
        .renderAreaOffset = { 0, 0 },
        .renderAreaExtent = environementMapSize,
        .clearBufferFlags = static_cast<decltype(renderPassBeginInfo.clearBufferFlags)>(BufferFlag::ColorBit | BufferFlag::DepthBit)
    };

    m_FrameBuffer->AttachTexture(brdfTexture, Attachment::Color00);

    m_PreComputeBrdr->Use();
    m_RenderPass.BeginRenderPass(renderPassBeginInfo);

    Rhi::DrawModel(DrawMode::Triangles, m_Quad->models[0]->GetId());

    m_RenderPass.EndRenderPass();
    m_PreComputeBrdr->Unuse();
    m_RenderPass.EndRenderPass();
}

void SkyBoxParser::Compute(const Texture& equirectangularMap, const Texture& cubemap, const Pointer<Shader>& shader)
{
    Resize(cubemap.GetSize());

    Matrix projection;
    Matrix::Perspective(90.f * Calc::Deg2Rad, 1.0f, 0.1f, 10.f, &projection);

    std::array<Matrix, static_cast<size_t>(CubeMapFace::Size)> captureViews;
    Rhi::GetCubeMapViewMatrices(&captureViews);

    shader->Use();
    shader->SetMat4("projection", projection);
    equirectangularMap.BindTexture(0);
    
    for (size_t i = 0; i < static_cast<size_t>(CubeMapFace::Size); i++)
    {
        RenderPassBeginInfo renderPassBeginInfo =
        {
            .frameBuffer = m_FrameBuffer,
            .renderAreaOffset = { 0,0 },
            .renderAreaExtent = m_TextureDepth->GetSize(),
            .clearBufferFlags = static_cast<decltype(renderPassBeginInfo.clearBufferFlags)>(BufferFlag::ColorBit | BufferFlag::DepthBit)
        };

        shader->SetMat4("view", captureViews[i]);

        m_FrameBuffer->AttachTextureLayer(cubemap, Attachment::Color00, 0, static_cast<uint32_t>(i));
        m_RenderPass.BeginRenderPass(renderPassBeginInfo);

        Rhi::DrawModel(DrawMode::Triangles, m_Cube->models[0]->GetId());

        m_RenderPass.EndRenderPass();
    }

    shader->Unuse();
}

void SkyBoxParser::Resize(const Vector2i size)
{
    if (m_TextureDepth == nullptr || m_TextureDepth->GetSize() != size)
    {
        delete m_TextureDepth;
    
        m_TextureDepth = new Texture(TextureInternalFormat::DepthComponent32, size);
        m_TextureDepth->CreateInInterface();
        m_FrameBuffer->AttachTexture(*m_TextureDepth, Attachment::Depth);
    }
}
