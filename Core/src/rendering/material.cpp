#include "rendering/material.hpp"

#include "rendering/rhi.hpp"

using namespace XnorCore;

void Material::BindMaterial() const
{
    if (albedoTexture.IsValid())
        albedoTexture->BindTexture(MaterialTextureEnum::Albedo);
		
    if (metallicTexture.IsValid())
        metallicTexture->BindTexture(MaterialTextureEnum::Metallic);

    if (roughnessTexture.IsValid())
        roughnessTexture->BindTexture(MaterialTextureEnum::Roughness);

    if (normalTexture.IsValid())
        normalTexture->BindTexture(MaterialTextureEnum::Normal);

    if (ambientOcclusionTexture.IsValid())
        ambientOcclusionTexture->BindTexture(MaterialTextureEnum::AmbiantOcclusion);

    if (emissiveTexture.IsValid())
        emissiveTexture->BindTexture(MaterialTextureEnum::EmissiveMap);

    Rhi::BindMaterial(*this);

}
