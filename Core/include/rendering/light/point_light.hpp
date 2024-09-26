#pragma once

#include "core.hpp"
#include "rendering/light/light.hpp"

/// @file point_light.hpp
/// @brief Defines the XnorCore::PointLight class

BEGIN_XNOR_CORE

/// @brief Handles a point light
class PointLight : public Light
{
    REFLECTABLE_IMPL(PointLight)
    
public:
    XNOR_ENGINE PointLight() = default;
    
    XNOR_ENGINE ~PointLight() override = default;

    DEFAULT_COPY_MOVE_OPERATIONS(PointLight)
};

END_XNOR_CORE

REFL_AUTO(type(XnorCore::PointLight, bases<XnorCore::Light>))
