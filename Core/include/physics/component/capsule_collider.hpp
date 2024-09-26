#pragma once

#include "core.hpp"
#include "scene/component.hpp"
#include "physics/component/collider.hpp"

/// @file capsule_collider.hpp
/// @brief Defines the XnorCore::CapsuleCollider class

BEGIN_XNOR_CORE

/// @brief Capsule collider
class CapsuleCollider : public Collider
{
public:

private:
    REFLECTABLE_IMPL(CapsuleCollider)

public:
    
    float_t height = 1.f;
    float_t radius = 1.f;
    
    XNOR_ENGINE CapsuleCollider() = default; 
    XNOR_ENGINE ~CapsuleCollider() override = default;
    
    DEFAULT_COPY_MOVE_OPERATIONS(CapsuleCollider)
    
    /// @brief Awake function
    XNOR_ENGINE void Awake() override;
    /// @brief Update function
    XNOR_ENGINE void Update() override;
    
    XNOR_ENGINE void Begin() override;

};

END_XNOR_CORE

REFL_AUTO(type(XnorCore::CapsuleCollider, bases<XnorCore::Collider>),
    field(height),
    field(radius)
)
