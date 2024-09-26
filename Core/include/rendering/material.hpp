#pragma once

#include <vector>

#include "core.hpp"
#include "reflection/reflection.hpp"
#include "resource/texture.hpp"
#include "utils/color.hpp"
#include "utils/pointer.hpp"

/// @file material.hpp
/// @brief Defines the XnorCore::Material struct

BEGIN_XNOR_CORE

/// @brief Material type
enum class MaterialType
{
    Opaque,
    Lit
};

/// @brief Encapsulates a material, which defines how a Model is rendered
struct Material
{
    REFLECTABLE_IMPL(Material)
    
public:
    // Pointer<Shader> shader;

    /// @brief Material type
    MaterialType materialType = MaterialType::Opaque;
    /// @brief Albedo texture
    Pointer<Texture> albedoTexture;
    /// @brief Metallic texture
    Pointer<Texture> metallicTexture;
    /// @brief Roughness texture
    Pointer<Texture> roughnessTexture;
    /// @brief Normal map
    Pointer<Texture> normalTexture;
    /// @brief Ambiant occlusion texture
    Pointer<Texture> ambientOcclusionTexture;

    Pointer<Texture> emissiveTexture;

    /// @brief Albedo color
    Color albedoColor;
    /// @brief Emissive color
    Color emissiveColor;
    
    /// @brief Metallic parameter
    float_t metallic = 0.f;
    /// @brief Roughness parameter
    float_t roughness = 0.f;
    /// @brief Reflectance parameter
    float_t reflectance = 0.f;
    /// @brief Emissive parameter
    float_t emissive = 0.f;
    /// @brief Ambient occlusion parameter
    float_t ambientOcclusion = 0.f;
    
    void XNOR_ENGINE BindMaterial() const;
};

END_XNOR_CORE

/// @private
REFL_AUTO(
    type(XnorCore::Material),
    field(materialType),
    field(albedoColor),
    field(emissiveColor),
    field(metallic, XnorCore::Reflection::Range(0.f, 1.f)),
    field(roughness, XnorCore::Reflection::Range(0.f, 1.f)),
    field(reflectance, XnorCore::Reflection::Range(0.f, 1.f)),
    field(emissive, XnorCore::Reflection::Range(0.f, 100.f)),
    field(ambientOcclusion, XnorCore::Reflection::Range(0.f, 1.f)),
    field(albedoTexture),
    field(metallicTexture),
    field(roughnessTexture),
    field(normalTexture),
    field(ambientOcclusionTexture),
    field(emissiveTexture)
);
