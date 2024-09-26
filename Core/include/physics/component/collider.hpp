#pragma once

#include "core.hpp"
#include "physics/data/collision_data.hpp"
#include "scene/component.hpp"
#include "utils/event.hpp"

/// @file collider.hpp
/// @brief Defines the XnorCore::Collider class

BEGIN_XNOR_CORE

/// @brief Base class for collider components
class Collider : public Component
{
    REFLECTABLE_IMPL(Collider)
public:
    /// @brief Collider constraints
    enum ColliderConstraints : size_t
    {
        /// @brief No constraints
        ConstraintNone = 0 << 0,
        /// @brief Position constraint
        ConstraintPosition = 1 << 0,
        /// @brief Rotation constraint
        ConstraintRotation = 1 << 1,
    };

    Vector3 center = Vector3::Zero();

    XNOR_ENGINE Collider() = default; 
    XNOR_ENGINE ~Collider() override;

    DEFAULT_COPY_MOVE_OPERATIONS(Collider)

    /// @brief Awake function
    XNOR_ENGINE void Awake() override{}
    
    /// @brief Pre-function
    XNOR_ENGINE void PrePhysics() override;
    /// @brief Post-function
    XNOR_ENGINE void PostPhysics() override;

    /// @brief Get whether the collider is a trigger
    /// @returns Is trigger
    [[nodiscard]]
    XNOR_ENGINE bool_t IsTrigger() const;

    /// @brief Adds a force to the component
    /// @param force Force
    XNOR_ENGINE void AddForce(const Vector3& force) const;

    /// @brief Adds an impulse to the component
    /// @param impulse Force
    XNOR_ENGINE void AddImpulse(const Vector3& impulse) const;

    /// @brief Set the friction of the body
    /// @param friction friction
    XNOR_ENGINE void SetFriction(float_t friction);

    XNOR_ENGINE void SetMass(float_t mass);


    /// @brief Get the friction of the body
    /// @param value value
    [[nodiscard]]
    XNOR_ENGINE float_t GetFriction() const;

    XNOR_ENGINE void SetLinearVelocity(Vector3 velocity);

    XNOR_ENGINE void AddLinearVelocity(Vector3 velocity);

    XNOR_ENGINE Vector3 GetLinearVelocity() const;
    
    XNOR_ENGINE void MoveKinematic(const Vector3& inTargetPosition, const Quaternion& inTargetRotation, float_t inDeltaTime);

    /// @brief Callback if the collider is a trigger and another collider entered in it
    Event<Collider*, Collider*, const CollisionData&> onTriggerEnter;
    /// @brief Callback if the collider is a trigger and another collider stayed in it
    Event<Collider*, Collider*, const CollisionData&> onTriggerStay;
    /// @brief Callback if the collider is a trigger and another collider left it
    Event<Collider*, Collider*> onTriggerExit;
    /// @brief Callback if the collider is not a trigger and another collider entered in it
    Event<Collider*, Collider*, const CollisionData&> onCollisionEnter;
    /// @brief Callback if the collider is not a trigger and another collider stayed in it
    Event<Collider*, Collider*, const CollisionData&> onCollisionStay;
    /// @brief Callback if the collider is not a trigger and another collider left in it
    Event<Collider*, Collider*> onCollisionExit;

    /// @brief Collider constraints
    ColliderConstraints constraints = ConstraintNone;

protected:
    /// @brief Adds the default debug events for the callbacks
    void AddDebugEvents();

    static void Test(Collider*, Collider*, const CollisionData&) {}
    
    /// @brief Jolt internal body ID for the collider
    uint32_t m_BodyId = std::numeric_limits<uint32_t>::max();

    /// @brief Whether the collider is static
    bool_t m_IsStatic = false;
    /// @brief Whether the collider is a trigger
    bool_t m_IsTrigger = false;
    /// @brief Whether the collider is active
    bool_t m_IsActive = false;

    float_t m_Friction = 0.f;

    float_t m_Mass = 1.f;

};

END_XNOR_CORE

REFL_AUTO(type(XnorCore::Collider, bases<XnorCore::Component>),
    field(center),
    field(constraints, XnorCore::Reflection::EnumFlags()),
    field(m_Friction,
        XnorCore::Reflection::Range(0.f, 10000.f),
        XnorCore::Reflection::ModifiedCallback<XnorCore::Collider>(
            [](XnorCore::Collider* collider)
            {
                collider->SetFriction(collider->m_Friction);
            }
        )
    ),
    field(m_Mass,
            XnorCore::Reflection::Range(0.f, 10000.f),
            XnorCore::Reflection::ModifiedCallback<XnorCore::Collider>(
                [](XnorCore::Collider* collider)
                {
                    collider->SetMass(collider->m_Mass);
                }
            )
        ),
    field(m_IsStatic),
    field(m_IsTrigger),
    field(m_IsActive, XnorCore::Reflection::ReadOnly()),
    field(onTriggerEnter, XnorCore::Reflection::NotSerializable())
)
