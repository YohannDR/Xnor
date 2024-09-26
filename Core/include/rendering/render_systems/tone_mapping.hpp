#pragma once

#include "core.hpp"
#include "rendering/frame_buffer.hpp"
#include "resource/mesh.hpp"
#include "resource/model.hpp"
#include "resource/shader.hpp"

/// @file tone_mapping.hpp
/// Defines the XnorCore::ToneMapping class

BEGIN_XNOR_CORE

/// @brief Encapsulates the ACES tone mapping algorithm
class ToneMapping
{
    static constexpr uint32_t ToneMapedTextureBinding = 10;

public:
    XNOR_ENGINE ToneMapping() = default;
    
    XNOR_ENGINE ~ToneMapping() = default;

    DEFAULT_COPY_MOVE_OPERATIONS(ToneMapping)

    /// @brief Initializes the shader resources
    XNOR_ENGINE void InitializeResources();
    
    /// @brief Recompiles the underlying shader
    XNOR_ENGINE void RecompileShader();
    
    /// @brief Computes the tone mapping
    /// @param imageWithoutToneMapping Initial image
    /// @param bloomTexture Bloom texture   
    XNOR_ENGINE void ComputeToneMaping(const Texture& imageWithoutToneMapping, const Texture& bloomTexture) const;

private:
    Pointer<Shader> m_Aces = nullptr;

    Pointer<Mesh> m_Quad = nullptr;
};

END_XNOR_CORE
