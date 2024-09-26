#pragma once

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/ObjectLayer.h>
#include <Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h>

#include "core.hpp"
#include "physics/layers.hpp"

BEGIN_XNOR_CORE

/// @private
class BroadPhaseLayerInterfaceImpl final : public JPH::BroadPhaseLayerInterface
{
public:
    XNOR_ENGINE BroadPhaseLayerInterfaceImpl();

    [[nodiscard]]
    XNOR_ENGINE JPH::uint GetNumBroadPhaseLayers() const override;

    [[nodiscard]]
    XNOR_ENGINE JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const override;

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
    XNOR_ENGINE virtual const char_t* GetBroadPhaseLayerName(JPH::BroadPhaseLayer inLayer) const override;
#endif // JPH_EXTERNAL_PROFILE || JPH_PROFILE_ENABLED

private:
    JPH::BroadPhaseLayer mObjectToBroadPhase[Layers::NUM_LAYERS];
};

END_XNOR_CORE
