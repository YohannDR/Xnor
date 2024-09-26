#include "transform.hpp"

#include "world/scene_graph.hpp"

using namespace XnorCore;

const Vector3& Transform::GetPosition() const
{
    return m_Position;
}

void Transform::SetPosition(const Vector3& newPosition)
{
    m_Position = newPosition;
    m_Changed = true;
}

void Transform::SetPositionX(const float_t newPositionX)
{
    m_Position.x = newPositionX;
    m_Changed = true;
}

void Transform::SetPositionY(const float_t newPositionY)
{
    m_Position.y = newPositionY;
    m_Changed = true;
}

void Transform::SetPositionZ(const float_t newPositionZ)
{
    m_Position.z = newPositionZ;
    m_Changed = true;
}

const Vector3& Transform::GetRotationEulerAngle() const
{
    return m_EulerRotation;
}

void Transform::SetRotationEulerAngle(const Vector3& newRotationEulerAngle)
{
    m_EulerRotation = newRotationEulerAngle;
    m_Changed = true;
}

void Transform::SetRotationEulerAngleX(const float_t newRotationEulerAngleX)
{
    m_EulerRotation.x = newRotationEulerAngleX;
    m_Changed = true;
}

void Transform::SetRotationEulerAngleY(const float_t newRotationEulerAngleY)
{
    m_EulerRotation.y = newRotationEulerAngleY;
    m_Changed = true;
}

void Transform::SetRotationEulerAngleZ(const float_t newRotationEulerAngleZ)
{
    m_EulerRotation.z = newRotationEulerAngleZ;
    m_Changed = true;
}

const Quaternion& Transform::GetRotation() const
{
    return m_Rotation;
}

void Transform::SetRotation(const Quaternion& newRotation)
{
    m_Rotation = newRotation;
    m_EulerRotation = Quaternion::ToEuler(m_Rotation);
    m_Changed = true;
}

void Transform::SetRotationX(const float_t newRotationX)
{
    m_Rotation.X() = newRotationX;
    m_EulerRotation = Quaternion::ToEuler(m_Rotation);
    m_Changed = true;
}

void Transform::SetRotationY(const float_t newRotationY)
{
    m_Rotation.Y() = newRotationY;
    m_EulerRotation = Quaternion::ToEuler(m_Rotation);
    m_Changed = true;
}

void Transform::SetRotationZ(const float_t newRotationZ)
{
    m_Rotation.Z() = newRotationZ;
    m_EulerRotation = Quaternion::ToEuler(m_Rotation);
    m_Changed = true;
}

void Transform::SetRotationW(const float_t newRotationW)
{
    m_Rotation.W() = newRotationW;
    m_Changed = true;
}

const Vector3& Transform::GetScale() const
{
    return m_Scale;
}


void Transform::SetScale(const Vector3& newScale)
{
    m_Scale = newScale;
    m_Changed = true;
}

void Transform::SetScaleX(const float_t newScaleX)
{
    m_Scale.x = newScaleX;
    m_Changed = true;
}

void Transform::SetScaleY(const float_t newScaleY)
{
    m_Scale.y = newScaleY;
    m_Changed = true;
}

void Transform::SetScaleZ(const float_t newScaleZ)
{
    m_Scale.z = newScaleZ;
    m_Changed = true;
}

bool_t Transform::GetChanged() const
{
    return m_Changed;
}

Vector3 Transform::GetRight() const
{
    return (Matrix3(worldMatrix) * Vector3::UnitX()).Normalized();
}

Vector3 Transform::GetUp() const
{
    return (Matrix3(worldMatrix) * Vector3::UnitY()).Normalized();
}

Vector3 Transform::GetForward() const
{
    return (Matrix3(worldMatrix) * -Vector3::UnitZ()).Normalized();
}
