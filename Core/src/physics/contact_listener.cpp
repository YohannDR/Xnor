#include "physics/contact_listener.hpp"

#include "Jolt/Physics/Body/Body.h"
#include "physics/physics_world.hpp"
#include "physics/data/collision_data.hpp"
#include "utils/logger.hpp"

using namespace XnorCore;

void ContactListenerImpl::ProcessEvents()
{
    for (size_t i = 0; i < EventCount; i++)
    {
        for (size_t j = 0; j < m_Events[i].size(); j++)
        {
            const EventInfo& info = m_Events[i][j];

            switch (i)
            {
                case TriggerEnter:
                    info.self->onTriggerEnter.Invoke(info.self, info.other, info.data);
                    break;

                case TriggerStay:
                    info.self->onTriggerStay.Invoke(info.self, info.other, info.data);
                    break;

                case TriggerExit:
                    info.self->onTriggerExit.Invoke(info.self, info.other);
                    break;

                case CollisionEnter:
                    info.self->onCollisionEnter.Invoke(info.self, info.other, info.data);
                    break;

                case CollisionStay:
                    info.self->onCollisionStay.Invoke(info.self, info.other, info.data);
                    break;

                case CollisionExit:
                    info.self->onCollisionExit.Invoke(info.self, info.other);
                    break;

                default:
                    break;
            }
        }
    }

    for (size_t i = 0; i < EventCount; i++)
        m_Events[i].clear();
}

JPH::ValidateResult ContactListenerImpl::OnContactValidate(
    [[maybe_unused]] const JPH::Body& inBody1,
    [[maybe_unused]] const JPH::Body& inBody2,
    [[maybe_unused]] const JPH::RVec3Arg inBaseOffset,
    [[maybe_unused]] const JPH::CollideShapeResult& inCollisionResult
)
{
    // Logger::LogInfo("Contact between {} and {}", inBody1.GetID().GetIndexAndSequenceNumber(), inBody2.GetID().GetIndexAndSequenceNumber());

    return JPH::ValidateResult::AcceptAllContactsForThisBodyPair;
}

void ContactListenerImpl::OnContactAdded(
    const JPH::Body& inBody1,
    const JPH::Body& inBody2,
    const JPH::ContactManifold& inManifold,
    [[maybe_unused]] JPH::ContactSettings& ioSettings
)
{
    const uint32_t bodyId1 = inBody1.GetID().GetIndexAndSequenceNumber();
    const uint32_t bodyId2 = inBody2.GetID().GetIndexAndSequenceNumber();
    Collider* const c1 = PhysicsWorld::GetColliderFromId(bodyId1);
    Collider* const c2 = PhysicsWorld::GetColliderFromId(bodyId2);

    const CollisionData data = {
        .penetrationDepth = inManifold.mPenetrationDepth,
        .normal = Vector3(inManifold.mWorldSpaceNormal.GetX(), inManifold.mWorldSpaceNormal.GetY(), inManifold.mWorldSpaceNormal.GetZ())
    };

    if (inBody1.IsSensor() && !inBody2.IsSensor())
    {
        std::scoped_lock lock(m_EventsMutex);
        // Body 1 is the trigger, so body 2 entered in it
        m_Events[TriggerEnter].emplace_back(c1, c2, data);
    }
    else if (!inBody1.IsSensor() && inBody2.IsSensor())
    {
        std::scoped_lock lock(m_EventsMutex);
        // Body 2 is the trigger, so body 1 entered in it
        m_Events[TriggerEnter].emplace_back(c2, c1, data);
    }
    else if (!inBody1.IsSensor() && !inBody2.IsSensor())
    {
        std::scoped_lock lock(m_EventsMutex);
        // Both body aren't triggers, so it's a normal collision
        // We call the events of both of them unlike the triggers
        m_Events[CollisionEnter].emplace_back(c1, c2, data);
        m_Events[CollisionEnter].emplace_back(c2, c1, data);
    }
    else
    {
        // TODO handle trigger + trigger collision
    }
}

void ContactListenerImpl::OnContactPersisted(
    const JPH::Body& inBody1,
    const JPH::Body& inBody2,
    const JPH::ContactManifold& inManifold,
    [[maybe_unused]] JPH::ContactSettings& ioSettings
)
{
    const uint32_t bodyId1 = inBody1.GetID().GetIndexAndSequenceNumber();
    const uint32_t bodyId2 = inBody2.GetID().GetIndexAndSequenceNumber();
    Collider* const c1 = PhysicsWorld::GetColliderFromId(bodyId1);
    Collider* const c2 = PhysicsWorld::GetColliderFromId(bodyId2);

    const CollisionData data = {
        .penetrationDepth = inManifold.mPenetrationDepth,
        .normal = Vector3(inManifold.mWorldSpaceNormal.GetX(), inManifold.mWorldSpaceNormal.GetY(), inManifold.mWorldSpaceNormal.GetZ())
    };
    
    if (inBody1.IsSensor() && !inBody2.IsSensor())
    {
        std::scoped_lock lock(m_EventsMutex);
        // Body 1 is the trigger, so body 2 entered in it
        m_Events[TriggerStay].emplace_back(c1, c2, data);
    }
    else if (!inBody1.IsSensor() && inBody2.IsSensor())
    {
        std::scoped_lock lock(m_EventsMutex);
        // Body 2 is the trigger, so body 1 entered in it
        m_Events[TriggerStay].emplace_back(c2, c1, data);
    }
    else if (!inBody1.IsSensor() && !inBody2.IsSensor())
    {
        std::scoped_lock lock(m_EventsMutex);
        // Both body aren't triggers, so it's a normal collision
        // We call the events of both of them unlike the triggers
        m_Events[CollisionStay].emplace_back(c1, c2, data);
        m_Events[CollisionStay].emplace_back(c2, c1, data);
    }
    else
    {
        // TODO handle trigger + trigger collision
    }
}

void ContactListenerImpl::OnContactRemoved(const JPH::SubShapeIDPair& inSubShapePair)
{
    const uint32_t bodyId1 = inSubShapePair.GetBody1ID().GetIndexAndSequenceNumber();
    const uint32_t bodyId2 = inSubShapePair.GetBody2ID().GetIndexAndSequenceNumber();

    Collider* const c1 = PhysicsWorld::GetColliderFromId(bodyId1);
    Collider* const c2 = PhysicsWorld::GetColliderFromId(bodyId2);

    constexpr CollisionData data;
    
    if (c1 == nullptr || c2 == nullptr)
        return;
    
    if (c1->IsTrigger() && !c2->IsTrigger())
    {
        std::scoped_lock lock(m_EventsMutex);
        // Body 1 is the trigger, so body 2 entered in it
        m_Events[TriggerExit].emplace_back(c1, c2, data);
    }
    else if (!c1->IsTrigger() && c2->IsTrigger())
    {
        std::scoped_lock lock(m_EventsMutex);
        // Body 2 is the trigger, so body 1 entered in it
        m_Events[TriggerExit].emplace_back(c2, c1, data);
    }
    else if (!c1->IsTrigger() && !c2->IsTrigger())
    {
        std::scoped_lock lock(m_EventsMutex);
        // Both body aren't triggers, so it's a normal collision
        // We call the events of both of them unlike the triggers
        m_Events[CollisionExit].emplace_back(c1, c2, data);
        m_Events[CollisionExit].emplace_back(c2, c1, data);
    }
}
