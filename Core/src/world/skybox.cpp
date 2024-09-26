#include "world/skybox.hpp"

#include "rendering/rhi.hpp"

using namespace XnorCore;

Skybox::~Skybox()
{
    delete m_CubeMapAlbedo;
    m_CubeMapAlbedo = nullptr;
    delete m_IrradianceMap;
    m_IrradianceMap = nullptr;
    delete m_PrefilterMap;
    m_IrradianceMap = nullptr;
    delete m_PrecomputeBrdfTexture;
    m_PrecomputeBrdfTexture = nullptr;
}

void Skybox::Initialize()
{
    delete m_CubeMapAlbedo;
    delete m_IrradianceMap;
    delete m_PrefilterMap;
    delete m_PrecomputeBrdfTexture;
    
    TextureCreateInfo createCubeMapInfo =
    {
        .textureType = TextureType::TextureCubeMap,
        .mipMaplevel = 0,
        .size = EnvironementCubeMapSize,
        .filtering = TextureFiltering::Linear,
        .wrapping = TextureWrapping::ClampToEdge,
        .format = TextureFormat::Rgb,
        .internalFormat = TextureInternalFormat::Rgb16F,
        .dataType = DataType::Float
    };
    m_CubeMapAlbedo = new Texture(createCubeMapInfo);
    
    createCubeMapInfo.size = IradianceCubeSize;
    m_IrradianceMap = new Texture(createCubeMapInfo);
    
    createCubeMapInfo.size = PrefilterMapSize;
    createCubeMapInfo.filtering = TextureFiltering::LinearMimMapLinear;
    m_PrefilterMap = new Texture(createCubeMapInfo);

    const TextureCreateInfo precomputeBrdf =
    {
        .textureType = TextureType::Texture2D,
        .size = EnvironementCubeMapSize,
        .filtering = TextureFiltering::Linear,
        .wrapping = TextureWrapping::ClampToEdge,
        .format = TextureFormat::RedGreen,
        .internalFormat = TextureInternalFormat::Rg16F,
        .dataType = DataType::Float
    };
    m_PrecomputeBrdfTexture = new Texture(precomputeBrdf);

    if (!m_SkyboxTextureResource)
        return;

    LoadFromHdrTexture(m_SkyboxTextureResource);
}

void Skybox::LoadFromHdrTexture(const Pointer<Texture>& hdrFile) 
{
    m_SkyboxTextureResource = hdrFile;
    Rhi::skyBoxParser.EquirectangularToCubeMapFunc(*hdrFile.Get(), *m_CubeMapAlbedo);
    Rhi::skyBoxParser.PreComputeBrdf(EnvironementCubeMapSize, *m_PrecomputeBrdfTexture);
    Rhi::skyBoxParser.ComputeIrradiance(*m_CubeMapAlbedo, *m_IrradianceMap, IradianceCubeSize);
    Rhi::skyBoxParser.ComputePreFiltering(*m_CubeMapAlbedo, *m_PrefilterMap, MaxMinMapLevel);
}

void Skybox::BindDesriptorSet() const 
{
    if (m_IrradianceMap)
        m_IrradianceMap->BindTexture(DefferedDescriptor::SkyboxIrradiance);
    if (m_PrefilterMap)
        m_PrefilterMap->BindTexture(DefferedDescriptor::SkyboxPrefilterMap);
    if (m_PrecomputeBrdfTexture)
        m_PrecomputeBrdfTexture->BindTexture(DefferedDescriptor::SkyboxPrecomputeBrdf);
}

void Skybox::UnbindDesriptorSet() const
{
    if (m_IrradianceMap)
        m_IrradianceMap->UnbindTexture(DefferedDescriptor::SkyboxIrradiance);
    if (m_PrefilterMap)
        m_PrefilterMap->UnbindTexture(DefferedDescriptor::SkyboxPrefilterMap);
    if (m_PrecomputeBrdfTexture)
        m_PrecomputeBrdfTexture->UnbindTexture(DefferedDescriptor::SkyboxPrecomputeBrdf);
}

const Texture* Skybox::GetSkyboxAlbedoColor() const
{
    return m_CubeMapAlbedo;
}

