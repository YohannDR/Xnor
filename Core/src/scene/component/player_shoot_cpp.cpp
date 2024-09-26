#include "scene/component/player_shoot_cpp.hpp"

#include "input/input.hpp"
#include "physics/component/sphere_collider.hpp"
#include "resource/resource_manager.hpp"
#include "scene/component/enemy_cpp.hpp"
#include "world/world.hpp"

using namespace XnorCore;

void PlayerShootCpp::SetupBullet(Entity* const bullet) const
{
    SphereCollider* coll = bullet->AddComponent<SphereCollider>();
    coll->SetRadius(m_BulletSize);
    coll->onCollisionEnter += [this](const Collider* const self, const Collider* const enemy, const CollisionData& data){ HitEnemy(self, enemy, data); };

    StaticMeshRenderer* const meshRenderer = bullet->AddComponent<StaticMeshRenderer>();
    meshRenderer->mesh = ResourceManager::Get<Mesh>("assets/models/sphere.obj");
    meshRenderer->material.emissiveColor = Color::Green();
    meshRenderer->material.emissive = 1000.f;

    Matrix shootPointMatrix = shootPoint->transform.worldMatrix;
    bullet->transform.SetPosition( { shootPointMatrix.m03, shootPointMatrix.m13, shootPointMatrix.m23 });
    bullet->transform.SetScale( bullet->transform.GetScale() * m_BulletSize);
    const Vector3 force = (bullet->transform.GetPosition() - parent->transform.GetPosition()).Normalized() * 10000.f;
    coll->AddForce(force);
}

void PlayerShootCpp::Shoot()
{
    if (!m_CanShoot)
        return;

    Entity* const bullet = World::scene->CreateEntity("Bullet");
    m_Bullets.Add({bullet, m_BulletLifeTime });
  
    SetupBullet(bullet);
    BulletSound(bullet);
    m_CanShoot = false;
    m_RestShoot = Coroutine::Start(ResetDirtyFlagShoot());
}

void PlayerShootCpp::BulletSound(Entity* const bullet)
{
    AudioSource* const audioSource = bullet->AddComponent<AudioSource>();
    audioSource->audioTrack = ResourceManager::Get<AudioTrack>("assets/sounds/blaster.wav");
    audioSource->SetLooping(false);
    audioSource->Play();
}

void PlayerShootCpp::HitEnemy(const Collider* const, const Collider* const other, const CollisionData&) const
{
    EnemyCpp* enemyCpp = nullptr;
    
    if (!other->entity->TryGetComponent(&enemyCpp))
        return;

    enemyCpp->TakeDamage(m_DmgOnHit);
}

void PlayerShootCpp::UpdateBullets()
{
    for (size_t i = 0; i < m_Bullets.GetSize(); i++)
    {
        std::pair<Entity*,float_t>& bullet = m_Bullets[i];
        
        bullet.second -= Time::GetDeltaTime<float_t>();

        if (bullet.second <= 0.f)
        {
            World::scene->DestroyEntity(bullet.first);
            m_Bullets.Remove(bullet);
            i--;
        }
    }
}

Coroutine PlayerShootCpp::ResetDirtyFlagShoot()
{
    using namespace std::chrono_literals;
    co_await m_FireRate;
    m_CanShoot = true;
}

PlayerShootCpp::~PlayerShootCpp()
{
    Coroutine::Stop(m_RestShoot);
}

void PlayerShootCpp::Awake()
{
}

void PlayerShootCpp::Begin()
{
    parent = entity->GetParent();
    shootPoint = entity->GetChild(0);
}

void PlayerShootCpp::Update()
{
    if (Input::GetMouseButton(MouseButton::Right, MouseButtonStatus::Down))
        Shoot();

    UpdateBullets();
}
