#pragma once

#include "rendering/light/light.hpp"

/// @file spot_light.hpp
/// @brief Defines the XnorCore::DirectionalLight class

BEGIN_XNOR_CORE

/// @brief Handles a spot light
class SpotLight : public Light
{
    REFLECTABLE_IMPL(SpotLight)

public:
    /// @brief Cut-off
    float_t cutOff = 12.5f;
    /// @brief Outer cut-off	
    float_t outerCutOff = 15.f;
    
    XNOR_ENGINE SpotLight() = default;
    XNOR_ENGINE ~SpotLight() override = default;

    DEFAULT_COPY_MOVE_OPERATIONS(SpotLight)

    /// @brief Gets the direction of the spot light
    /// @returns Light direction
    [[nodiscard]]
    XNOR_ENGINE Vector3 GetLightDirection() const;
};

END_XNOR_CORE

REFL_AUTO(type(XnorCore::SpotLight, bases<XnorCore::Light>),
    field(cutOff),
    field(outerCutOff)
)
