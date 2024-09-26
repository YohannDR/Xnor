#include "utils/bound.hpp"

using namespace XnorCore;

Bound:: Bound(const Vector3 newCenter, const Vector3 newSize) : extents(newSize * 0.5f),
center(newCenter)
{
}

bool_t Bound::operator==(const Bound& other) const
{
    return center == other.center && extents == other.extents; 
}

Vector3 Bound::GetMin() const
{
    return center - extents;
}

Vector3 Bound::GetMax() const
{
    return center + extents;
}

Vector3 Bound::GetSize() const
{
    return extents * 2.f; 
}

void Bound::SetMinMax(const Vector3 newmin, const Vector3 newMax)
{
    extents = (newMax - newmin) * 0.5f;
    center = newmin + extents;
}

Bound Bound::GetAabbFromTransform(const Bound& bound, const Transform& transform)
{
    const Vector3 globalPos = static_cast<Vector3>(transform.worldMatrix * Vector4(bound.center.x, bound.center.y, bound.center.z, 1.f));
    // Let the constructor
    return ReturnAabbFromMatrix(bound, transform.worldMatrix, globalPos);
}

bool_t Bound::Intersect(const Bound& otherBound) const
{
    const Vector3 max = GetMax();
    const Vector3 min = GetMin();

    const Vector3 otherMax = otherBound.GetMax();
    const Vector3 otherMin = otherBound.GetMin();

    const bool_t xOverlap = min.x <= otherMax.x && max.x >= otherMin.x;
    const bool_t yOverlap = min.y <= otherMax.y && max.y >= otherMin.y;
    const bool_t zOverlap = min.z <= otherMax.z && max.z >= otherMin.z;

    return xOverlap && yOverlap && zOverlap;
}

bool_t Bound::Countain(const Bound& otherBound) const
{
    const Vector3 max = GetMax();
    const Vector3 min = GetMin();

    const Vector3 otherMax = otherBound.GetMax();
    const Vector3 otherMin = otherBound.GetMin();

    const bool_t xInside = min.x <= otherMin.x && max.x >= otherMax.x;
    const bool_t yInside = min.y <= otherMin.y && max.y >= otherMax.y;
    const bool_t zInside = min.z <= otherMin.z && max.z >= otherMax.z;

    return xInside && yInside && zInside;

}

bool_t Bound::IsOnPlane(const Plane& plane) const
{
    const float_t r = extents.x * std::abs(plane.normal.x) +
            extents.y * std::abs(plane.normal.y) + extents.z * std::abs(plane.normal.z);

    const float_t distance = plane.GetSignedDistanceToPlane(center);

    return -r <= distance;
}
    
void Bound::Encapsulate(const Bound& encapsulateBound)
{
    Encapsulate(encapsulateBound.center - encapsulateBound.extents);
    Encapsulate(encapsulateBound.center + encapsulateBound.extents);
}

void Bound::Encapsulate(const Vector3 point)
{
    const Vector3 min = GetMin();
    const Vector3 max = GetMax();
    
    const float_t minX = std::min(min.x, point.x);
    const float_t minY = std::min(min.y, point.y);
    const float_t minZ = std::min(min.z, point.z);
    
    const float_t maxX = std::max(max.x, point.x);
    const float_t maxY = std::max(max.y, point.y);
    const float_t maxZ = std::max(max.z, point.z);

    SetMinMax({ minX, minY, minZ }, { maxX, maxY, maxZ });
}

Bound Bound::ReturnAabbFromMatrix(const Bound& bound,const Matrix& matrix, const Vector3 center)
{
    const Vector3 right = static_cast<Vector3>(matrix[0]) * bound.extents.x;
    const Vector3 up =  static_cast<Vector3>(matrix[1]) * bound.extents.y;
    const Vector3 forward =  static_cast<Vector3>(matrix[2]) * bound.extents.z;
    
    const float_t newExtendX = std::abs(Vector3::Dot(Vector3::UnitX(),right)) +
       std::abs(Vector3::Dot(Vector3::UnitX(),up)) + std::abs(Vector3::Dot(Vector3::UnitX(),forward));

    const float_t newExtendY = std::abs(Vector3::Dot(Vector3::UnitY(),right)) +
       std::abs(Vector3::Dot(Vector3::UnitY(),up)) + std::abs(Vector3::Dot(Vector3::UnitY(),forward));

    const float_t newExtendZ = std::abs(Vector3::Dot(Vector3::UnitZ(),right)) +
       std::abs(Vector3::Dot(Vector3::UnitZ(),up)) + std::abs(Vector3::Dot(Vector3::UnitZ(),forward));

    return Bound(center, Vector3(newExtendX, newExtendY, newExtendZ) * 2.f);
}
