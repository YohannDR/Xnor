#pragma once

#include "core.hpp"

#include <Maths/vector3.hpp>

/// @file collision_data.hpp
/// @brief Defines the XnorCore::CollisionData struct

BEGIN_XNOR_CORE

/// @brief Collision data that's provided in the collision callbacks
struct CollisionData
{
    /// @brief Penetration depth
    float_t penetrationDepth = 0.f;
    /// @brief Collision normal
    Vector3 normal;
    // Vector3 contactPoints;
};

END_XNOR_CORE
