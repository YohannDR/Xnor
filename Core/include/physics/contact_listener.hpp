#pragma once

#include "core.hpp"

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/ContactListener.h>

#include "physics/component/collider.hpp"

BEGIN_XNOR_CORE

/// @private
class ContactListenerImpl final : public JPH::ContactListener
{
public:
    void ProcessEvents();
    
private:
    JPH::ValidateResult OnContactValidate(const JPH::Body& inBody1, const JPH::Body& inBody2, JPH::RVec3Arg inBaseOffset, const JPH::CollideShapeResult& inCollisionResult) override;
    void OnContactAdded(const JPH::Body& inBody1, const JPH::Body& inBody2, const JPH::ContactManifold& inManifold, JPH::ContactSettings& ioSettings) override;
    void OnContactPersisted(const JPH::Body& inBody1, const JPH::Body& inBody2, const JPH::ContactManifold& inManifold, JPH::ContactSettings& ioSettings) override;
    void OnContactRemoved(const JPH::SubShapeIDPair& inSubShapePair) override;

    struct EventInfo
    {
        Collider* self = nullptr;
        Collider* other = nullptr;
        CollisionData data{};

        constexpr EventInfo(Collider* s, Collider* o, const CollisionData& d)
            : self(s), other(o), data(d)
        {}
    };

    enum CollisionEvent
    {
        TriggerEnter,
        TriggerStay,
        TriggerExit,
        CollisionEnter,
        CollisionStay,
        CollisionExit,

        EventCount
    };

    std::mutex m_EventsMutex;
    std::array<std::vector<EventInfo>, EventCount> m_Events;
};

END_XNOR_CORE
