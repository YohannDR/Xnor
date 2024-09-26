#include "scene/component/enemy_cpp.hpp"

#include "Jolt/Core/Core.h"
#include "physics/component/sphere_collider.hpp"
#include "scene/entity.hpp"
#include "world/world.hpp"

using namespace XnorCore;

void EnemyCpp::OnDetectionEnter(Collider* const, const Collider* const other, const CollisionData&)
{
    if (other->entity->name != "Player")
        return;

    Logger::LogInfo("Malphite will follow this entity = {}", other->entity->name);
    m_SkinnedMeshRenderer->StartAnimation(m_Run);
    player = other->entity;
    m_IsInDetectionRange = true;
}

void EnemyCpp::OnDetectionExit(Collider* const, const Collider* const other)
{
    if (other->entity->name != "Player")
        return;

    Logger::LogInfo("Malphite Lost interest = {}", other->entity->name);
    m_SkinnedMeshRenderer->StartAnimation(m_Idle);
    player = other->entity;
    m_IsInDetectionRange = false;
    capsule->SetLinearVelocity(Vector3::Zero());
}

void EnemyCpp::OnTriggerStay(Collider* const, const Collider* const other, const CollisionData&)
{
    if (other->entity->name != "Player")
        return;

    LookAtPlayer();

    if (m_IsAttacking)
    {
        capsule->SetLinearVelocity(Vector3::Zero());
        return;
    }
    
    if (IsInRange())
    {
        Attack();
    }
    else
    {
        Move();
    }
}

void EnemyCpp::Move() const
{
    if (entity == nullptr)
        return;
    
    // Update velocity
    const Vector3 currentVelocity = capsule->GetLinearVelocity();
    Vector3 desiredVelocity = m_FwdVector * m_MoveSpeed;
    desiredVelocity.y = currentVelocity.y;
    const Vector3 newVelocity = 0.75f * currentVelocity + 0.25f * desiredVelocity;
    
    // Update position
    capsule->SetLinearVelocity(newVelocity);
}

void EnemyCpp::LookAtPlayer()
{
    const Vector3 pos1 = GetTransform().GetPosition();
    const Vector3 pos2 = player->transform.GetPosition();
    const Vector3 fow = (pos2 - pos1).Normalized();
    Vector3 fowLook = fow;
    fowLook.y = 0.f;
    m_FwdVector = fow;
    const Vector3 targetPosition = pos1 + fowLook;
    entity->LookAt(pos1, targetPosition);
}

bool_t EnemyCpp::IsInRange() const
{
    return (entity->transform.GetPosition() - player->transform.GetPosition()).Length() <= m_AttackRange;
}

void EnemyCpp::Attack()
{
    m_SkinnedMeshRenderer->StartAnimation(m_Attack);
    m_IsAttacking = true;
    m_ResetDirtyFlagAttackRoutineGuid = Coroutine::Start(ResetDirtyFlagAttackRoutine());
}

Coroutine EnemyCpp::ResetDirtyFlagAttackRoutine()
{
    using namespace std::chrono_literals;
    co_await 2s;
    m_IsAttacking = false;
}

Coroutine EnemyCpp::ResetDirtyFlagIsInvicible()
{
    using namespace std::chrono_literals;
    co_await 2s;
    m_IsInvincible = false;
    m_SkinnedMeshRenderer->material.emissive = 0.f;
}


EnemyCpp::~EnemyCpp()
{
    Coroutine::Stop(m_ResetDirtyFlagAttackRoutineGuid);
    Coroutine::Stop(m_ResetDirtyFlagIsInvicibleRoutineGuid);
}

void EnemyCpp::Awake()
{
    Component::Awake();
    SphereCollider* const s = entity->GetComponent<SphereCollider>();
    capsule = entity->GetComponent<CapsuleCollider>();
    
    s->onTriggerEnter += [this](Collider* const self, const Collider* const other, const CollisionData& data) { OnDetectionEnter(self, other, data); };
    s->onTriggerExit += [this](Collider* const self, const Collider* const other) { OnDetectionExit(self, other); };
    s->onTriggerStay += [this](Collider* const self, const Collider* const other, const CollisionData& data) { OnTriggerStay(self, other, data); };

    m_SkinnedMeshRenderer = entity->GetComponent<SkinnedMeshRenderer>();
    m_SkinnedMeshRenderer->StartAnimation(m_Idle);
}

void EnemyCpp::Update()
{
    Component::Update();
}

void EnemyCpp::OnRendering()
{
    Component::OnRendering();
    DrawGizmo::Sphere(entity->transform.GetPosition() ,m_AttackRange, Color::Red());
}

void EnemyCpp::TakeDamage(const float_t dmg)
{
    if (m_IsInvincible)
        return;
    
    m_LifePoint -= dmg;
    m_IsInvincible = true;

    if (m_LifePoint <= 0.f)
    {
        World::scene->DestroyEntity(entity);
        return;
    }

    m_SkinnedMeshRenderer->material.emissiveColor = Color::Red();
    m_SkinnedMeshRenderer->material.emissive = 1000.f;
    m_ResetDirtyFlagIsInvicibleRoutineGuid =  Coroutine::Start(ResetDirtyFlagIsInvicible());
}
