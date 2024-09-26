#pragma once

#include "core.hpp"
#include "Maths/vector3.hpp"

BEGIN_XNOR_CORE

struct XNOR_ENGINE Plane
{
    Vector3 normal = Vector3::UnitY();
    float_t distance = 0.f;

    Plane(Vector3 p1, Vector3 newNormal);

    Plane() = default;

    ~Plane() = default;

    DEFAULT_COPY_MOVE_OPERATIONS(Plane)
    
    float_t GetSignedDistanceToPlane(const Vector3& point) const;
};

END_XNOR_CORE