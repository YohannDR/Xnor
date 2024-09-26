#pragma once

#include "core.hpp"
#include "rendering/render_pass.hpp"
#include "resource/mesh.hpp"
#include "resource/model.hpp"
#include "resource/shader.hpp"
#include "resource/texture.hpp"

/// @file skybox_parser.hpp
/// @brief Defines the XnorCore::SkyBoxParser class

BEGIN_XNOR_CORE

/// @brief Class that handles processing a skybox
class SkyBoxParser
{
public:
    XNOR_ENGINE SkyBoxParser() = default;
    
    XNOR_ENGINE ~SkyBoxParser();

    DEFAULT_COPY_MOVE_OPERATIONS(SkyBoxParser)

    /// @brief Initializes the skybox parser
    XNOR_ENGINE void Init();

    /// @brief Converts an equirectangular map to a cubemap
    /// @param equirectangularMap Source equirectangular map
    /// @param cubemap Target cubemap
    XNOR_ENGINE void EquirectangularToCubeMapFunc(const Texture& equirectangularMap, const Texture& cubemap);

    /// @brief Computes the irradiance
    /// @param irradianceInput Source cubemap
    /// @param irradianceOutput Target cubemap
    /// @param irradianceSize Irradiance size
    XNOR_ENGINE void ComputeIrradiance(const Texture& irradianceInput, const Texture& irradianceOutput, Vector2i irradianceSize);

    /// @brief Computes the pre-filtering
    /// @param environementMap Source cubemap
    /// @param prefilteringMap Target cubemap
    /// @param mipLevels Mip levels
    XNOR_ENGINE void ComputePreFiltering(const Texture& environementMap, const Texture& prefilteringMap, uint32_t mipLevels);

    /// @brief Computes the BRDF
    /// @param environementMapSize Map size
    /// @param brdfTexture BRDF texture
    XNOR_ENGINE void PreComputeBrdf(Vector2i environementMapSize, const Texture& brdfTexture);

private:
    Pointer<Mesh> m_Cube;
    
    Pointer<Mesh> m_Quad;

    Framebuffer* m_FrameBuffer = nullptr;
    RenderPass m_RenderPass;
    
    Pointer<Shader> m_EquirectangularToCubeMapShader;
    
    Pointer<Shader> m_IrradianceConvolution;
    
    Pointer<Shader> m_PrefilterShader;
    
    Pointer<Shader> m_PreComputeBrdr;

    Texture* m_TextureDepth = nullptr;
    
    Texture* m_Color = nullptr;
    
    XNOR_ENGINE void Resize(Vector2i size);
    
    XNOR_ENGINE void Compute(const Texture& equirectangularMap, const Texture& cubemap, const Pointer<Shader>& shader);
};

END_XNOR_CORE
