#pragma once
#include "core.hpp"
#include "skinned_mesh_renderer.hpp"
#include "physics/component/capsule_collider.hpp"
#include "resource/animation.hpp"
#include "scene/component.hpp"
#include "utils/coroutine.hpp"
#include "utils/pointer.hpp"

BEGIN_XNOR_CORE
    struct CollisionData;
    class Collider;

class EnemyCpp : public Component
{
    REFLECTABLE_IMPL(EnemyCpp)
public:
    DEFAULT_COPY_MOVE_OPERATIONS(EnemyCpp)

    XNOR_ENGINE EnemyCpp() = default;

    XNOR_ENGINE ~EnemyCpp();

    XNOR_ENGINE void Awake() override;
    
    XNOR_ENGINE void Update() override;
    
    XNOR_ENGINE void OnRendering() override;

    XNOR_ENGINE void TakeDamage(float_t dmg);
    
private:
    Pointer<Animation> m_Idle;

    Pointer<Animation> m_Run;

    Pointer<Animation> m_Attack;

    SkinnedMeshRenderer* m_SkinnedMeshRenderer = nullptr;

    bool_t m_IsInDetectionRange;

    const Entity* player;

    CapsuleCollider* capsule = nullptr;

    float_t m_MoveSpeed = 1.f;

    float_t KillY = -1000.f;

    float_t m_AttackRange = 1.f;

    bool_t m_IsAttacking = false;

    float_t m_LifePoint = 100.f;
    
    bool_t m_IsInvincible = false;

    Vector3 m_FwdVector;
    
    XNOR_ENGINE void OnDetectionEnter(Collider* coll1, const Collider* other, const CollisionData& data);

    XNOR_ENGINE void OnDetectionExit(Collider* coll1, const Collider* other);
    
    XNOR_ENGINE void OnTriggerStay(Collider* coll1, const Collider* other, const CollisionData& data);
    
    XNOR_ENGINE void Move() const;

    // Return The forward Vector
    XNOR_ENGINE void LookAtPlayer();

    [[nodiscard]]
    XNOR_ENGINE bool_t IsInRange() const;

    XNOR_ENGINE void Attack();

    XNOR_ENGINE Coroutine ResetDirtyFlagAttackRoutine();

    XNOR_ENGINE Coroutine ResetDirtyFlagIsInvicible();

    Guid m_ResetDirtyFlagAttackRoutineGuid;

    Guid m_ResetDirtyFlagIsInvicibleRoutineGuid;
};

END_XNOR_CORE

REFL_AUTO(
    type(XnorCore::EnemyCpp, bases<XnorCore::Component>),
    field(m_Idle),
    field(m_Run),
    field(m_Attack),
    field(m_MoveSpeed),
    field(m_AttackRange),
    field(m_LifePoint)
);
