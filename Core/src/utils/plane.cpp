#include "utils/plane.hpp"

using namespace XnorCore;

Plane::Plane(const Vector3 p1, const Vector3 newNormal) : normal(newNormal.Normalized()),distance(Vector3::Dot(normal,p1))
{
    
}

float_t Plane::GetSignedDistanceToPlane(const Vector3& point) const
{
    return Vector3::Dot(normal, point) - distance;
}
