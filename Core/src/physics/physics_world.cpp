#include "physics/physics_world.hpp"

#include <cstdarg>

#include <Jolt/Jolt.h>
#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Collision/CastResult.h>
#include <Jolt/Physics/Collision/RayCast.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/CapsuleShape.h>
#include <Jolt/Physics/Collision/Shape/ConvexHullShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Collision/Shape/CompoundShape.h>

#include "input/time.hpp"
#include "jolt/Physics/Character/Character.h"
#include "Maths/matrix.hpp"
#include "utils/logger.hpp"

using namespace XnorCore;

using namespace JPH::literals;

namespace Layers
{
    static constexpr JPH::ObjectLayer NON_MOVING = 0;
    static constexpr JPH::ObjectLayer MOVING = 1;
    static constexpr JPH::ObjectLayer NUM_LAYERS = 2;
}

JPH::Vec3Arg PhysicsWorld::ToJph(const Vector3& in)
{
    return JPH::RVec3Arg(in.x, in.y, in.z);
}

JPH::QuatArg PhysicsWorld::ToJph(const Quaternion& in)
{
    return JPH::QuatArg(in.X(), in.Y(), in.Z(), in.W());
}

Vector3 PhysicsWorld::FromJph(const JPH::Vec3& in)
{
    return Vector3(in.GetX(), in.GetY(), in.GetZ());
}

Quaternion PhysicsWorld::FromJph(const JPH::Quat& in)
{
    return Quaternion(in.GetX(), in.GetY(), in.GetZ(), in.GetW());
}


void PhysicsWorld::Initialize()
{
    // Register allocation hook
    JPH::RegisterDefaultAllocator();

    // Install callbacks
    JPH::Trace = TraceImpl;

    // Create a factory
    JPH::Factory::sInstance = new JPH::Factory();

    // Register all Jolt physics types
    JPH::RegisterTypes();

    // We need a temp allocator for temporary allocations during the physics update. We're
    // pre-allocating 10 MB to avoid having to do allocations during the physics update.
    // B.t.w. 10 MB is way too much for this example but it is a typical value you can use.
    // If you don't want to pre-allocate you can also use TempAllocatorMalloc to fall back to
    // malloc / free.
    m_Allocator = new JPH::TempAllocatorImpl(10 * 1024 * 1024);
    
    // We need a job system that will execute physics jobs on multiple threads. Typically
    // you would implement the JobSystem interface yourself and let Jolt Physics run on top
    // of your own job scheduler. JobSystemThreadPool is an example implementation.
    m_JobSystem = new JPH::JobSystemThreadPool(JPH::cMaxPhysicsJobs, JPH::cMaxPhysicsBarriers, static_cast<int32_t>(std::thread::hardware_concurrency()) - 1);

    // This is the max amount of rigid bodies that you can add to the physics system. If you try to add more you'll get an error.
    // Note: This value is low because this is a simple test. For a real project use something in the order of 65536.
    constexpr JPH::uint maxBodies = 1024;

    // This determines how many mutexes to allocate to protect rigid bodies from concurrent access. Set it to 0 for the default settings.
    constexpr JPH::uint numBodyMutexes = 0;

    // This is the max amount of body pairs that can be queued at any time (the broad phase will detect overlapping
    // body pairs based on their bounding boxes and will insert them into a queue for the narrowphase). If you make this buffer
    // too small the queue will fill up and the broad phase jobs will start to do narrow phase work. This is slightly less efficient.
    // Note: This value is low because this is a simple test. For a real project use something in the order of 65536.
    constexpr JPH::uint maxBodyPairs = 1024;

    // This is the maximum size of the contact constraint buffer. If more contacts (collisions between bodies) are detected than this
    // number then these contacts will be ignored and bodies will start interpenetrating / fall through the world.
    // Note: This value is low because this is a simple test. For a real project use something in the order of 10240.
    constexpr JPH::uint maxContactConstraints = 1024;

    // Now we can create the actual physics system.
    m_PhysicsSystem = new JPH::PhysicsSystem();
    m_PhysicsSystem->Init(maxBodies, numBodyMutexes, maxBodyPairs, maxContactConstraints, m_BroadPhaseLayerInterface, m_ObjectVsBroadphaseLayerFilter, m_ObjectVsObjectLayerFilter);

    // A body activation listener gets notified when bodies activate and go to sleep
    // Note that this is called from a job so whatever you do here needs to be thread safe.
    // Registering one is entirely optional.
    m_PhysicsSystem->SetBodyActivationListener(&m_BodyActivationListener);

    // A contact listener gets notified when bodies (are about to) collide, and when they separate again.
    // Note that this is called from a job so whatever you do here needs to be thread safe.
    // Registering one is entirely optional.
    m_PhysicsSystem->SetContactListener(&m_ContactListener);

    // The main way to interact with the bodies in the physics system is through the body interface. There is a locking and a non-locking
    // variant of this. We're going to use the locking version (even though we're not planning to access bodies from multiple threads)
    m_BodyInterface = &m_PhysicsSystem->GetBodyInterface();

    SetGravity(Vector3(0.f, -9.81f, 0.f));
}

void PhysicsWorld::Destroy()
{
    delete m_Allocator;
    delete m_JobSystem;
    delete m_PhysicsSystem;
    
    // Unregisters all types with the factory and cleans up the default material
    JPH::UnregisterTypes();

    // Destroy the factory
    delete JPH::Factory::sInstance;
    JPH::Factory::sInstance = nullptr;
}

void PhysicsWorld::Update(const float_t deltaTime)
{
    m_PhysicsSystem->Update(deltaTime, 1, m_Allocator, m_JobSystem);
    m_ContactListener.ProcessEvents();
}

void PhysicsWorld::SetGravity(const Vector3& gravity)
{
    m_PhysicsSystem->SetGravity(JPH::Vec3Arg(gravity.x, gravity.y, gravity.z));
}

uint32_t PhysicsWorld::CreateSphere(const BodyCreationInfo& info, const float_t radius)
{
    JPH::BodyCreationSettings settings(new JPH::SphereShape(radius), ToJph(info.position), JPH::Quat::sIdentity(), JPH::EMotionType::Dynamic, Layers::MOVING);

    return CreateBody(info, settings);
}

uint32_t PhysicsWorld::CreateBox(const BodyCreationInfo& info)
{
    JPH::BodyCreationSettings settings(new JPH::BoxShape(ToJph(info.scaling)), ToJph(info.position), ToJph(info.rotation), JPH::EMotionType::Dynamic, Layers::MOVING);

    return CreateBody(info, settings);
}

JPH::Character* PhysicsWorld::CreateCharacter(const BodyCreationInfo& info, const JPH::CharacterSettings& settings)
{
    JPH::Character* const c = new JPH::Character(&settings, ToJph(info.position), ToJph(info.rotation), 0, m_PhysicsSystem);
    c->AddToPhysicsSystem(JPH::EActivation::Activate);
    
    const uint32_t bodyId = c->GetBodyID().GetIndexAndSequenceNumber();
    m_BodyMap.emplace(bodyId, info.collider);

    return c;
}

uint32_t PhysicsWorld::CreateCapsule(const BodyCreationInfo& info, const float_t height, const float_t radius)
{
    const JPH::CapsuleShapeSettings capsuleSettings(height, radius);
    const JPH::ShapeSettings::ShapeResult result = capsuleSettings.Create();

    if (!result.IsValid())
    {
        Logger::LogError("[Physics] - Couldn't create the capsule shape");
        return JPH::BodyID::cInvalidBodyID;
    }
    JPH::BodyCreationSettings settings(result.Get(), ToJph(info.position), ToJph(info.rotation), JPH::EMotionType::Dynamic, Layers::MOVING);

    return CreateBody(info, settings);
}

uint32_t PhysicsWorld::CreateConvexHull(const BodyCreationInfo& info, const std::vector<Vertex>& vertices)
{
    std::vector<JPH::Vec3> positions(vertices.size());

    for (size_t i = 0; i < vertices.size(); i++)
        positions[i] = ToJph(vertices[i].position);
    
    JPH::ConvexHullShapeSettings hullSettings(positions.data(), static_cast<int32_t>(positions.size()), JPH::cDefaultConvexRadius);

    const JPH::ShapeSettings::ShapeResult result = hullSettings.Create();
    if (!result.IsValid())
    {
        Logger::LogError("[Physics] - Couldn't create the convex hull shape");
        return JPH::BodyID::cInvalidBodyID;
    }
    
    JPH::BodyCreationSettings settings(result.Get(), ToJph(info.position), ToJph(info.rotation), JPH::EMotionType::Dynamic, Layers::MOVING);

    return CreateBody(info, settings);
}

void PhysicsWorld::DestroyBody(const uint32_t bodyId)
{
    m_BodyInterface->RemoveBody(JPH::BodyID(bodyId));
    m_BodyInterface->DestroyBody(JPH::BodyID(bodyId));
    m_BodyMap.erase(m_BodyMap.find(bodyId));
}

Vector3 PhysicsWorld::GetBodyPosition(const uint32_t bodyId)
{
    const JPH::BodyID id = JPH::BodyID(bodyId);

    if (!IsBodyActive(bodyId))
    {
        Logger::LogWarning("[Physics] - Trying to get the position of an inactive body : {}", bodyId);
        return Vector3::Zero();
    }

    const JPH::RVec3 position = m_BodyInterface->GetCenterOfMassPosition(id);

    return FromJph(position);
}

Quaternion PhysicsWorld::GetBodyRotation(uint32_t bodyId)
{
    const JPH::BodyID id = JPH::BodyID(bodyId);

    if (!IsBodyActive(bodyId))
    {
        Logger::LogWarning("[Physics] - Trying to get the rotation of an inactive body : {}", bodyId);
        return Quaternion::Identity();
    }

    const JPH::Quat rotation = m_BodyInterface->GetRotation(id);
    return FromJph(rotation);
}

void PhysicsWorld::SetPosition(const uint32_t bodyId, const Vector3& position)
{
    m_BodyInterface->SetPosition(JPH::BodyID(bodyId), JPH::RVec3Arg(position.x, position.y, position.z), JPH::EActivation::DontActivate);
}

void PhysicsWorld::SetRotation(const uint32_t bodyId, const Quaternion& rotation)
{
    m_BodyInterface->SetRotation(JPH::BodyID(bodyId), JPH::QuatArg(rotation.X(), rotation.Y(), rotation.Z(), rotation.W()), JPH::EActivation::DontActivate);
}

void PhysicsWorld::AddForce(const uint32_t bodyId, const Vector3& force)
{
    const JPH::BodyLockWrite lock(m_PhysicsSystem->GetBodyLockInterface(), JPH::BodyID(bodyId));

    if (!lock.Succeeded())
        return;

    JPH::Body& body = lock.GetBody();
    body.AddForce(ToJph(force) / body.GetMotionProperties()->GetInverseMass());
}

void PhysicsWorld::AddForce(const uint32_t bodyId, const Vector3& force, const Vector3& point)
{
    const JPH::BodyLockWrite lock(m_PhysicsSystem->GetBodyLockInterface(), JPH::BodyID(bodyId));

    if (!lock.Succeeded())
        return;

    JPH::Body& body = lock.GetBody();

    body.AddForce(ToJph(force) / body.GetMotionProperties()->GetInverseMass(), ToJph(point));
}

void PhysicsWorld::AddImpulse(const uint32_t bodyId, const Vector3& impulse)
{
    const JPH::BodyLockWrite lock(m_PhysicsSystem->GetBodyLockInterface(), JPH::BodyID(bodyId));

    if (!lock.Succeeded())
        return;

    JPH::Body& body = lock.GetBody();

    body.AddImpulse(ToJph(impulse) * body.GetMotionProperties()->GetInverseMass());
}

void PhysicsWorld::AddImpulse(const uint32_t bodyId, const Vector3& impulse, const Vector3& point)
{
    const JPH::BodyLockWrite lock(m_PhysicsSystem->GetBodyLockInterface(), JPH::BodyID(bodyId));

    if (!lock.Succeeded())
        return;

    JPH::Body& body = lock.GetBody();

    body.AddImpulse(ToJph(impulse) * body.GetMotionProperties()->GetInverseMass(), ToJph(point));
}

void PhysicsWorld::SetFriction(uint32_t bodyId, float_t friction)
{
    const JPH::BodyLockWrite lock(m_PhysicsSystem->GetBodyLockInterface(), JPH::BodyID(bodyId));

    if (!lock.Succeeded())
        return;

    JPH::Body& body = lock.GetBody();

    body.SetFriction(friction);
}

float_t PhysicsWorld::GetFriction(uint32_t bodyId)
{
    const JPH::BodyLockWrite lock(m_PhysicsSystem->GetBodyLockInterface(), JPH::BodyID(bodyId));

    if (!lock.Succeeded())
        return 0.f;

    JPH::Body& body = lock.GetBody();

    return body.GetFriction();
}

void PhysicsWorld::AddTorque(const uint32_t bodyId, const Vector3& torque)
{
    m_BodyInterface->AddTorque(JPH::BodyID(bodyId), ToJph(torque));
}

Collider* PhysicsWorld::GetColliderFromId(const uint32_t bodyId)
{
    const auto&& it = m_BodyMap.find(bodyId);

    if (it == m_BodyMap.end())
        return nullptr;

    return it->second;
}

bool_t PhysicsWorld::Raycast(const Vector3& position, const Vector3& direction, const float_t length, RaycastResult* result)
{
    const JPH::RRayCast ray = JPH::RRayCast(ToJph(position), ToJph(direction.Normalized() * length));
    JPH::RayCastResult jphResult;

    const bool_t hit = m_PhysicsSystem->GetNarrowPhaseQuery().CastRay(ray, jphResult);

    const JPH::BodyLockRead lock(m_PhysicsSystem->GetBodyLockInterface(), JPH::BodyID(jphResult.mBodyID));

    if (!lock.Succeeded())
        return hit;

    const JPH::Body& body = lock.GetBody();

    result->hitBody = GetColliderFromId(jphResult.mBodyID.GetIndexAndSequenceNumber());
    result->point = FromJph(ray.GetPointOnRay(jphResult.mFraction));
    result->normal = FromJph(body.GetWorldSpaceSurfaceNormal(jphResult.mSubShapeID2, ray.GetPointOnRay(jphResult.mFraction)));
    result->distance = length * jphResult.mFraction;
    
    return hit;
}

void PhysicsWorld::SetLinearVelocity(uint32_t bodyId, Vector3 velocity)
{
    const JPH::BodyLockWrite lock(m_PhysicsSystem->GetBodyLockInterface(), JPH::BodyID(bodyId));

    if (!lock.Succeeded())
        return;

    JPH::Body& body = lock.GetBody();
    
    
    body.SetLinearVelocity(ToJph(velocity));
}

void PhysicsWorld::AddLinearVelocity(uint32_t bodyId, Vector3 velocity)
{
    const JPH::BodyLockWrite lock(m_PhysicsSystem->GetBodyLockInterface(), JPH::BodyID(bodyId));

    if (!lock.Succeeded())
        return;

    JPH::Body& body = lock.GetBody();

    body.SetLinearVelocity(ToJph(velocity) + body.GetLinearVelocity());
}

Vector3 PhysicsWorld::GetLinearVelocity(uint32_t bodyId)
{
    const JPH::BodyLockWrite lock(m_PhysicsSystem->GetBodyLockInterface(), JPH::BodyID(bodyId));

    if (!lock.Succeeded())
        return Vector3::Zero();

    JPH::Body& body = lock.GetBody();

    return FromJph(body.GetLinearVelocity());
}

void PhysicsWorld::MoveKinematic(uint32_t bodyId, Vector3 inTargetPosition, Quaternion inTargetRotation, float_t inDeltaTime)
{
    const JPH::BodyLockWrite lock(m_PhysicsSystem->GetBodyLockInterface(), JPH::BodyID(bodyId));

    if (!lock.Succeeded())
        return;

    JPH::Body& body = lock.GetBody();
    body.MoveKinematic(ToJph(inTargetPosition),ToJph(inTargetRotation),inDeltaTime);
}

void PhysicsWorld::SetInverseMass(uint32_t bodyId, float_t invertedMass)
{
    const JPH::BodyLockWrite lock(m_PhysicsSystem->GetBodyLockInterface(), JPH::BodyID(bodyId));

    if (!lock.Succeeded())
        return;

    JPH::Body& body = lock.GetBody();

    body.GetMotionProperties()->SetInverseMass(invertedMass);
}

bool_t PhysicsWorld::IsBodyActive(const uint32_t bodyId)
{
    return m_BodyInterface->IsActive(JPH::BodyID(bodyId));
}

void PhysicsWorld::TraceImpl(const char_t* format, ...)
{
    // Format the message
    va_list list;
    va_start(list, format);
    char buffer[1024];
    (void)vsnprintf(buffer, sizeof(buffer), format, list);  // NOLINT(clang-diagnostic-format-nonliteral)
    va_end(list);

    // Print to the TTY
    const char_t* const buf = buffer;
    Logger::LogInfo("{}", buf);
}

uint32_t PhysicsWorld::CreateBody(const BodyCreationInfo& info, JPH::BodyCreationSettings& settings)
{
    settings.mIsSensor = info.isTrigger;
    if (info.isTrigger || info.isStatic)
        settings.mMotionType = JPH::EMotionType::Static;

    settings.mAllowSleeping = false;

    const uint32_t bodyId = m_BodyInterface->CreateAndAddBody(settings, JPH::EActivation::Activate).GetIndexAndSequenceNumber();

    m_BodyMap.emplace(bodyId, info.collider);
    
    return bodyId;
}
