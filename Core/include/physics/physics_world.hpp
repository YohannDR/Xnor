#pragma once

#include "core.hpp"

#include <unordered_map>

#include <Jolt/Jolt.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSystem.h>

#include <Maths/quaternion.hpp>
#include <Maths/vector3.hpp>

#include "jolt/Physics/Character/Character.h"
#include "physics/body_activation_listener.hpp"
#include "physics/broad_phase_layer_interface.hpp"
#include "physics/contact_listener.hpp"
#include "physics/component/collider.hpp"
#include "rendering/vertex.hpp"

/// @file physics_world.hpp
/// @brief Defines the XnorCore::PhysicsWorld class

BEGIN_XNOR_CORE

/// @brief Provides helper functions to handle the physics
class PhysicsWorld
{
    STATIC_CLASS(PhysicsWorld)

public:
    /// @brief Body creation info
    struct BodyCreationInfo
    {
        /// @brief Attached collider
        Collider* collider = nullptr;
        
        /// @brief Position
        Vector3 position;
        /// @brief Rotation
        Quaternion rotation;
        /// @brief Scaling
        Vector3 scaling;

        /// @brief Whether it's a trigger
        bool_t isTrigger{};
        /// @brief Whether it's static
        bool_t isStatic{};

        Vector3 offsetShape;
    };

    /// @brief Raycast result
    struct RaycastResult
    {
        /// @brief Hit collider
        Collider* hitBody = nullptr;
        /// @brief Point of collision
        Vector3 point;
        /// @brief Surface normal
        Vector3 normal;
        /// @brief Distance between cast and collision
        float_t distance{};
    };

    [[nodiscard]]
    static JPH::Vec3Arg ToJph(const Vector3& in);
    [[nodiscard]]
    static JPH::QuatArg ToJph(const Quaternion& in);
    [[nodiscard]]
    static Vector3 FromJph(const JPH::Vec3& in);
    [[nodiscard]]
    static Quaternion FromJph(const JPH::Quat& in);



    /// @brief Initializes the physics world
    XNOR_ENGINE static void Initialize();
    /// @brief Destroys the physics world
    XNOR_ENGINE static void Destroy();
    /// @brief Updates the physics world
    /// @param deltaTime Delta time
    XNOR_ENGINE static void Update(float_t deltaTime);

    /// @brief Sets the gravity
    /// @param gravity Gravity
    XNOR_ENGINE static void SetGravity(const Vector3& gravity);

    /// @brief Creates a sphere body
    /// @param info Body creation info
    /// @param radius Radius
    /// @returns Created body id
    [[nodiscard]]
    XNOR_ENGINE static uint32_t CreateSphere(const BodyCreationInfo& info, float_t radius);

    /// @brief Creates a box body
    /// @param info Body creation info
    /// @returns Created body id
    [[nodiscard]]
    XNOR_ENGINE static uint32_t CreateBox(const BodyCreationInfo& info);

    [[nodiscard]]
    XNOR_ENGINE static JPH::Character* CreateCharacter(const BodyCreationInfo& info, const JPH::CharacterSettings& settings);

    /// @brief Creates a capsule body
    /// @param info Body creation info
    /// @param height Height
    /// @param radius Radius
    /// @returns Created body id
    [[nodiscard]]
    XNOR_ENGINE static uint32_t CreateCapsule(const BodyCreationInfo& info, float_t height, float_t radius);

    /// @brief Creates a convex hull body
    /// @param info Body creation info
    /// @param vertices Vertices
    /// @returns Created body id
    [[nodiscard]]
    XNOR_ENGINE static uint32_t CreateConvexHull(const BodyCreationInfo& info, const std::vector<Vertex>& vertices);

    /// @brief Destroys a body
    /// @param bodyId Body ID
    XNOR_ENGINE static void DestroyBody(uint32_t bodyId);

    /// @brief Checks whether the body is active
    /// @param bodyId Body ID
    /// @returns Whether the body is active
    [[nodiscard]]
    XNOR_ENGINE static bool_t IsBodyActive(uint32_t bodyId);

    /// @brief Gets the position of a body
    /// @param bodyId Body ID
    /// @returns Position
    [[nodiscard]]
    XNOR_ENGINE static Vector3 GetBodyPosition(uint32_t bodyId);

    /// @brief Gets the rotation of a body
    /// @param bodyId Body ID
    /// @returns Rotation
    [[nodiscard]]
    XNOR_ENGINE static Quaternion GetBodyRotation(uint32_t bodyId);
    
    /// @brief Sets the position of a body
    /// @param bodyId Body ID
    /// @param position Position
    XNOR_ENGINE static void SetPosition(uint32_t bodyId, const Vector3& position);
    
    /// @brief Sets the rotation of a body
    /// @param bodyId Body ID
    /// @param rotation Rotation
    XNOR_ENGINE static void SetRotation(uint32_t bodyId, const Quaternion& rotation);

    /// @brief Adds a force to a body
    /// @param bodyId Body ID
    /// @param force Force
    XNOR_ENGINE static void AddForce(uint32_t bodyId, const Vector3& force);

    /// @brief Adds a force to a body at a specific point
    /// @param bodyId Body ID
    /// @param force Force
    /// @param point Point
    XNOR_ENGINE static void AddForce(uint32_t bodyId, const Vector3& force, const Vector3& point);

    /// @brief Adds an impulse to a body
    /// @param bodyId Body ID
    /// @param impulse Force
    XNOR_ENGINE static void AddImpulse(uint32_t bodyId, const Vector3& impulse);

    /// @brief Adds a impulse to a body at a specific point
    /// @param bodyId Body ID
    /// @param impulse Impulse
    /// @param point Point
    XNOR_ENGINE static void AddImpulse(uint32_t bodyId, const Vector3& impulse, const Vector3& point);
    
    /// @brief Adds a impulse to a body at a specific point
    /// @param bodyId Body ID
    /// @param friction friction
    XNOR_ENGINE static void SetFriction(uint32_t bodyId, float_t friction);

    /// @brief Adds a impulse to a body at a specific point
    /// @param bodyId Body ID
    XNOR_ENGINE static float_t GetFriction(uint32_t bodyId);
    
    /// @brief Adds a torque to a body
    /// @param bodyId Body ID
    /// @param torque Torque
    XNOR_ENGINE static void AddTorque(uint32_t bodyId, const Vector3& torque);

    /// @brief Gets the associated collider from a body id
    /// @param bodyId Body ID
    /// @returns Collider
    [[nodiscard]]
    XNOR_ENGINE static Collider* GetColliderFromId(uint32_t bodyId);

    /// @brief Performs a raycast
    /// @param position Position
    /// @param direction Direction
    /// @param length Length
    /// @param result Raycast result
    /// @returns Whether a hit occured
    XNOR_ENGINE static bool_t Raycast(const Vector3& position, const Vector3& direction, float_t length, RaycastResult* result);
    
    XNOR_ENGINE static void SetLinearVelocity(uint32_t bodyId,Vector3 velocity);

    XNOR_ENGINE static void AddLinearVelocity(uint32_t bodyId,Vector3 velocity);

    XNOR_ENGINE static Vector3 GetLinearVelocity(uint32_t bodyId);

    XNOR_ENGINE static void MoveKinematic(uint32_t bodyId, Vector3 inTargetPosition, Quaternion inTargetRotation, float_t inDeltaTime);

    XNOR_ENGINE static void SetInverseMass(uint32_t bodyId, float_t invertedMass);

private:
    XNOR_ENGINE static void TraceImpl(const char_t* format, ...);

    [[nodiscard]]
    XNOR_ENGINE static uint32_t CreateBody(const BodyCreationInfo& info, JPH::BodyCreationSettings& settings);

    static inline std::unordered_map<uint32_t, Collider*> m_BodyMap;

    XNOR_ENGINE static inline JPH::PhysicsSystem* m_PhysicsSystem;
    XNOR_ENGINE static inline JPH::TempAllocatorImpl* m_Allocator;
    XNOR_ENGINE static inline JPH::JobSystemThreadPool* m_JobSystem;

    XNOR_ENGINE static inline BroadPhaseLayerInterfaceImpl m_BroadPhaseLayerInterface;
    XNOR_ENGINE static inline JPH::ObjectVsBroadPhaseLayerFilter m_ObjectVsBroadphaseLayerFilter;
    XNOR_ENGINE static inline JPH::ObjectLayerPairFilter m_ObjectVsObjectLayerFilter;
    XNOR_ENGINE static inline BodyActivationListenerImpl m_BodyActivationListener;
    XNOR_ENGINE static inline ContactListenerImpl m_ContactListener;

    XNOR_ENGINE static inline JPH::BodyInterface* m_BodyInterface;
};

END_XNOR_CORE
