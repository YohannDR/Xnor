#pragma once

#include "core.hpp"
#include "scene/component.hpp"
#include "utils/color.hpp"

/// @file light.hpp
/// @brief Defines the XnorCore::Light class

BEGIN_XNOR_CORE

/// @brief Base class for lights
class Light : public Component
{
    REFLECTABLE_IMPL(Light)
    
public:
    /// @brief Color of the light
    Color color = Color(1.f, 1.f, 1.f);
    
    /// @brief Intensity of the light
    float_t intensity = 1.f;

    /// @brief Whether the light should cast shadows
    bool_t castShadow = true;

    /// @brief Near clipping of the light
    float_t near = 0.01f;

    /// @brief Far clipping of the light
    float_t far = 1000.f;

    XNOR_ENGINE Light() = default;
    
    XNOR_ENGINE ~Light() override = default;

    DEFAULT_COPY_MOVE_OPERATIONS(Light)
};

END_XNOR_CORE

REFL_AUTO(type(XnorCore::Light, bases<XnorCore::Component>),
    field(color),
    field(intensity),
    field(castShadow)
)
