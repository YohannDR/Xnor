#pragma once

#include "core.hpp"
#include "scene/component.hpp"
#include "physics/component/collider.hpp"

/// @file box_collider.hpp
/// @brief Defines the XnorCore::BoxCollider class

BEGIN_XNOR_CORE

/// @brief Box collider
class BoxCollider : public Collider
{
public:
    void Awake() override;

private:
    REFLECTABLE_IMPL(BoxCollider)

public:
    XNOR_ENGINE BoxCollider() = default; 
    XNOR_ENGINE ~BoxCollider() override = default;
    
    DEFAULT_COPY_MOVE_OPERATIONS(BoxCollider)

    Vector3 size = Vector3(1.f);
};

END_XNOR_CORE

REFL_AUTO(type(XnorCore::BoxCollider, bases<XnorCore::Collider>),
    field(size)
)
