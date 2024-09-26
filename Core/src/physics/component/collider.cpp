#include "physics/component/collider.hpp"

#include "input/input.hpp"
#include "physics/physics_world.hpp"
#include "scene/entity.hpp"
#include "serialization/serializer.hpp"

using namespace XnorCore;


Collider::~Collider()
{
    if (!JPH::BodyID(m_BodyId).IsInvalid())
        PhysicsWorld::DestroyBody(m_BodyId);
}


void Collider::PrePhysics()
{
    m_IsActive = PhysicsWorld::IsBodyActive(m_BodyId);

    if (m_IsActive)
    {
        PhysicsWorld::SetPosition(m_BodyId, entity->transform.GetPosition() + center);
        PhysicsWorld::SetRotation(m_BodyId, entity->transform.GetRotation().Normalized());
    } 
}

void Collider::PostPhysics()
{
    if (!m_IsActive)
        return;

    if (m_IsTrigger)
        return;
    
    if (!(constraints & ConstraintPosition))
        entity->transform.SetPosition(PhysicsWorld::GetBodyPosition(m_BodyId) - center);

    if (!(constraints & ConstraintRotation))
        entity->transform.SetRotation((PhysicsWorld::GetBodyRotation(m_BodyId)).Normalized());
}

bool_t Collider::IsTrigger() const
{
    return m_IsTrigger;
}

void Collider::AddForce(const Vector3& force) const
{
    PhysicsWorld::AddForce(m_BodyId, force);
}

void Collider::AddImpulse(const Vector3& impulse) const
{
    PhysicsWorld::AddImpulse(m_BodyId, impulse);
}

void Collider::SetFriction(const float_t friction)
{
    m_Friction = friction;
    PhysicsWorld::SetFriction(m_BodyId, friction);
}

void Collider::SetMass(float_t mass)
{
    PhysicsWorld::SetInverseMass(m_BodyId,1.f/mass);
}

float_t Collider::GetFriction() const
{
    return PhysicsWorld::GetFriction(m_BodyId);
}

void Collider::SetLinearVelocity(const Vector3 velocity)
{
    PhysicsWorld::SetLinearVelocity(m_BodyId,velocity);
}

void Collider::AddLinearVelocity(const Vector3 velocity)
{
    PhysicsWorld::AddLinearVelocity(m_BodyId, velocity);
}

Vector3 Collider::GetLinearVelocity() const
{
    return PhysicsWorld::GetLinearVelocity(m_BodyId);
}

void Collider::MoveKinematic(const Vector3& inTargetPosition, const Quaternion& inTargetRotation,float_t inDeltaTime)
{
    PhysicsWorld::MoveKinematic(m_BodyId, inTargetPosition, inTargetRotation, inDeltaTime);
}

void Collider::AddDebugEvents()
{
    onTriggerEnter += [](const Collider* const self, const Collider* const other, const CollisionData& data) -> void
    {
        Logger::LogDebug("OnTriggerEnter between {} and {} ; Normal : {} ; Pen depth : {}", self->entity->name, other->entity->name, data.normal, data.penetrationDepth);
    };

    onTriggerStay += [](const Collider* const self, const Collider* const other, const CollisionData& data) -> void
    {
        Logger::LogDebug("OnTriggerStay between {} and {} ; Normal : {} ; Pen depth : {}", self->entity->name, other->entity->name, data.normal, data.penetrationDepth);
    };

    onTriggerExit += [](const Collider* const self, const Collider* const other) -> void
    {
        Logger::LogDebug("OnTriggerExit between {} and {}", self->entity->name, other->entity->name);
    };

    onCollisionEnter += [](const Collider* const self, const Collider* const other, const CollisionData& data) -> void
    {
        Logger::LogDebug("OnCollisionEnter between {} and {} ; Normal : {} ; Pen depth : {}", self->entity->name, other->entity->name, data.normal, data.penetrationDepth);
    };

    onCollisionStay += [](const Collider* const self, const Collider* const other, const CollisionData& data) -> void
    {
        Logger::LogDebug("OnCollisionStay between {} and {} ; Normal : {} ; Pen depth : {}", self->entity->name, other->entity->name, data.normal, data.penetrationDepth);
    };

    onCollisionExit += [](const Collider* const self, const Collider* const other) -> void
    {
        Logger::LogDebug("OnCollisionExit between {} and {}", self->entity->name, other->entity->name);
    };
}
