#pragma once

#include <Maths/vector3.hpp>

#include "core.hpp"
#include "transform.hpp"
#include "utils/plane.hpp"

BEGIN_XNOR_CORE

/// @brief Struct used to store the minimum and maximum bounds of the AABB of a Model.
class XNOR_ENGINE Bound
{
public:
    static Bound GetAabbFromTransform(const Bound& bound,const Transform& transform);
    
     /// @brief The extents of the Bounding Box. This is half size of the Bounds.
    Vector3 extents;
    /// @brief The center bound of this Bound.
    Vector3 center = Vector3::Zero();

    DEFAULT_COPY_MOVE_OPERATIONS(Bound)
    
    Bound(Vector3 newCenter, Vector3 newSize);
    
    Bound() = default;

    ~Bound() = default;

    bool_t operator==(const Bound& other) const;

    [[nodiscard]]
    Vector3 GetMin() const;

    [[nodiscard]]
    Vector3 GetMax() const;

    [[nodiscard]]
    Vector3 GetSize() const;

    void SetMinMax(Vector3 newmin, Vector3 newMax);

    [[nodiscard]]
    bool_t Intersect(const Bound& otherBound) const;

    [[nodiscard]]
    bool_t Countain(const Bound& otherBound) const;

    [[nodiscard]]
    bool_t IsOnPlane(const Plane& plane) const;

    void Encapsulate(const Bound& encapsulateBound);

    void Encapsulate(Vector3 point);

private:

    static Bound ReturnAabbFromMatrix(const Bound& bound, const Matrix& matrix, const Vector3 center);
};

END_XNOR_CORE
