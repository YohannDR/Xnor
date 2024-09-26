#include "rendering/light/spot_light.hpp"

#include <Maths/matrix.hpp>
#include <Maths/matrix3.hpp>

#include "scene/entity.hpp"

Vector3 XnorCore::SpotLight::GetLightDirection() const
{
    Matrix&& rot = Matrix(Matrix3(entity->transform.worldMatrix));
    rot.m33 = 1.f;
    
    Vector4 vec = rot * Vector4::UnitY();

    return { vec.x , vec.y , vec.z }; 
}
