#include "physics/component/capsule_collider.hpp"

#include "physics/physics_world.hpp"
#include "physics/component/collider.hpp"
#include "scene/entity.hpp"

using namespace XnorCore;

void CapsuleCollider::Begin()
{
    Collider::Begin();
}

void CapsuleCollider::Awake()
{
    const Transform& t = entity->transform;

    const PhysicsWorld::BodyCreationInfo info = {
        .collider = this,
        .position = t.GetPosition(),
        .rotation = t.GetRotation(),
        .scaling = t.GetScale(),
        .isTrigger = m_IsTrigger,
        .isStatic = m_IsStatic,
        .offsetShape = center
    };

    m_BodyId = PhysicsWorld::CreateCapsule(info, height, radius);
    PrePhysics();
}

void CapsuleCollider::Update()
{
    Collider::Update();
}
