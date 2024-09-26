#pragma once

#include "core.hpp"
#include "scene/component.hpp"
#include "physics/component/collider.hpp"

/// @file mesh_collider.hpp
/// @brief Defines the XnorCore::MeshCollider class

BEGIN_XNOR_CORE

/// @brief Mesh collider
class MeshCollider : public Collider
{
    REFLECTABLE_IMPL(MeshCollider)

public:
    XNOR_ENGINE MeshCollider() = default; 
    XNOR_ENGINE ~MeshCollider() override = default;

    /// @brief Begin function
    XNOR_ENGINE void Begin() override;
    /// @brief Update function
    XNOR_ENGINE void Update() override;
    
    DEFAULT_COPY_MOVE_OPERATIONS(MeshCollider)
};

END_XNOR_CORE

REFL_AUTO(type(XnorCore::MeshCollider, bases<XnorCore::Collider>))
