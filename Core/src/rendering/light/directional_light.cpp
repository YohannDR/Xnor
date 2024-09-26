#include "rendering/light/directional_light.hpp"

#include "Maths/matrix.hpp"
#include "scene/entity.hpp"

using namespace XnorCore;

DirectionalLight::DirectionalLight()
{
    near = DefaulNearFar.x;
    far = DefaulNearFar.y;
}

Vector3 DirectionalLight::GetLightDirection() const 
{
    Matrix&& rot = Matrix(Matrix3(entity->transform.worldMatrix));
    rot.m33 = 1.f;
    
    Vector4 vec = rot * Vector4::UnitY();

    return { vec.x , vec.y , vec.z }; 
}
