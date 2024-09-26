#pragma once

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>

#include "core.hpp"

BEGIN_XNOR_CORE

/// @private
class BodyActivationListenerImpl : public JPH::BodyActivationListener
{
public:
    XNOR_ENGINE void OnBodyActivated(const JPH::BodyID& inBodyId, JPH::uint64 inBodyUserData) override;

    XNOR_ENGINE void OnBodyDeactivated(const JPH::BodyID& inBodyId, JPH::uint64 inBodyUserData) override;
};

END_XNOR_CORE
