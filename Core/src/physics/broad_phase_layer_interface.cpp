#include "physics/broad_phase_layer_interface.hpp"

using namespace XnorCore;

namespace BroadPhaseLayers
{
    static constexpr JPH::BroadPhaseLayer NON_MOVING(0);
    static constexpr JPH::BroadPhaseLayer MOVING(1);
    static constexpr JPH::uint NUM_LAYERS(2);
};

BroadPhaseLayerInterfaceImpl::BroadPhaseLayerInterfaceImpl()
{
    // Create a mapping table from object to broad phase layer
    mObjectToBroadPhase[Layers::NON_MOVING] = BroadPhaseLayers::NON_MOVING;
    mObjectToBroadPhase[Layers::MOVING] = BroadPhaseLayers::MOVING;
}

JPH::uint BroadPhaseLayerInterfaceImpl::GetNumBroadPhaseLayers() const
{
	return BroadPhaseLayers::NUM_LAYERS;
}

JPH::BroadPhaseLayer BroadPhaseLayerInterfaceImpl::GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const
{
    JPH_ASSERT(inLayer < Layers::NUM_LAYERS);
    return mObjectToBroadPhase[inLayer];
}

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
const char_t* BroadPhaseLayerInterfaceImpl::GetBroadPhaseLayerName(const JPH::BroadPhaseLayer inLayer) const
{
    switch (static_cast<JPH::BroadPhaseLayer::Type>(inLayer))
    {
        case static_cast<JPH::BroadPhaseLayer::Type>(Layers::NON_MOVING):
            return "NON_MOVING";

        case static_cast<JPH::BroadPhaseLayer::Type>(Layers::MOVING):
            return "MOVING";

        default:
            JPH_ASSERT(false);
            return "INVALID";
    }
}
#endif // JPH_EXTERNAL_PROFILE || JPH_PROFILE_ENABLED
