#pragma once
#include "core.hpp"
#include "audio/component/audio_listener.hpp"
#include "audio/component/audio_source.hpp"
#include "physics/component/collider.hpp"
#include "scene/component.hpp"
#include "utils/coroutine.hpp"

BEGIN_XNOR_CORE

class PlayerShootCpp : public Component
{
    REFLECTABLE_IMPL(PlayerShootCpp)

public:
    PlayerShootCpp() = default;
    ~PlayerShootCpp() override;

    DEFAULT_COPY_MOVE_OPERATIONS(PlayerShootCpp)

    void Awake() override;
    
    void Begin() override;
    
    void Update() override;
    
private:
    Entity* parent = nullptr;
    
    Entity* shootPoint = nullptr;
    
    float_t m_BulletSize = 0.2f;

    float_t m_DmgOnHit = 25.f;

    List<std::pair<Entity*,float_t>> m_Bullets;

    float_t m_BulletLifeTime = 5.f;

    float_t m_FireRate = 2.5f;

    bool_t m_CanShoot = true;

    Guid m_RestShoot;

    void SetupBullet(Entity* bullet) const;
    
    void Shoot();

    void BulletSound(Entity* bullet);

    void HitEnemy(const Collider* self, const Collider* other, const CollisionData& data) const;

    void UpdateBullets();

    Coroutine ResetDirtyFlagShoot();
};

END_XNOR_CORE

/// @private
REFL_AUTO(
    type(XnorCore::PlayerShootCpp, bases<XnorCore::Component>),
    field(m_FireRate)
);
