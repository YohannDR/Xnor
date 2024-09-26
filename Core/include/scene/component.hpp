#pragma once

#include "core.hpp"
#include "reflection/reflection.hpp"

/// @file component.hpp
/// @brief Defines the XnorCore::Component class.

BEGIN_XNOR_CORE

class Entity;
class Transform;

/// @brief Represents a behavior that can be attached to an Entity.
class XNOR_ENGINE Component
{
    REFLECTABLE_IMPL(Component)

public:
    bool_t enabled = true;

    /// @brief Entity bound to the component
    Entity* entity = nullptr;
    
    Component() = default;

    virtual ~Component() = 0;

    DEFAULT_COPY_MOVE_OPERATIONS(Component)

    /// @brief Awake the component
    virtual void Awake() {}
    
    /// @brief Begins the component
    virtual void Begin() {}

    /// @brief Updates the component
    virtual void Update() {}

    /// @brief Called before the physics update
    virtual void PrePhysics() {}

    /// @brief Called after the physics update
    virtual void PostPhysics() {}

    virtual void OnRendering() {} 

    /// @brief Deletes this Component. This is only used for polymorphism purposes with the ScriptComponent type.
    virtual void Destroy();

    /// @brief Get the Entity on which this Component is attached
    [[nodiscard]]
    const Entity* GetEntity() const;

#ifndef SWIG
    /// @brief Get the Entity on which this Component is attached
    Entity* GetEntity();
#endif
    
    /// @brief Get the Transform attached to the Entity of this Component
    ///
    /// Shortcut for equivalent to:
    /// @code
    /// GetEntity().transform
    /// @endcode
    ///
    /// @see GetEntity
    const Transform& GetTransform() const;

#ifndef SWIG
    /// @brief Get the Transform attached to the Entity of this Component
    ///
    /// Shortcut for equivalent to:
    /// @code
    /// GetEntity().transform
    /// @endcode
    ///
    /// @see GetEntity
    Transform& GetTransform();
#endif
    
private:

    // We need Entity to be able to set m_Entity
    friend class Entity;
};

END_XNOR_CORE

/// @private
REFL_AUTO(type(XnorCore::Component),
    field(enabled),
    field(entity, XnorCore::Reflection::HideInInspector())
);
