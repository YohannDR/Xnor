#pragma once

#include <vector>

#include "core.hpp"
#include "transform.hpp"
#include "reflection/reflection.hpp"
#include "resource/model.hpp"
#include "scene/component.hpp"
#include "utils/concepts.hpp"
#include "utils/guid.hpp"
#include "utils/list.hpp"

/// @file entity.hpp
/// @brief Defines the XnorCore::Entity class.

BEGIN_XNOR_CORE

/// @brief Represents an object of the engine, behaviors can be attached to it via a list of Component
class Entity
{
    REFLECTABLE_IMPL(Entity)

public:
    XNOR_ENGINE Entity() = default;
    XNOR_ENGINE ~Entity();

    DEFAULT_COPY_MOVE_OPERATIONS(Entity)
    
    /// @brief Transform of the entity
    Transform transform;
    /// @brief Name of the entity
    std::string name;

    /// @brief Adds a Component to the entity
    /// @tparam T Component type
    /// @return Created component
    template <Concepts::ComponentT T>
    T* AddComponent();

    /// @brief Adds a raw Component to the entity
    /// @param component Component
    XNOR_ENGINE void AddComponent(Component* component);

    /// @brief Gets a specified Component
    /// @tparam T Component type
    /// @return Component, @c nullptr if it doesn't exists
    template <Concepts::ComponentT T>
    [[nodiscard]]
    T* GetComponent();

    /// @brief Gets a specified Component
    /// @tparam T Component type
    /// @return Component, @c nullptr if it doesn't exists
    template <Concepts::ComponentT T>
    [[nodiscard]]
    const T* GetComponent() const;
    
    /// @brief Gets all of the specified Component
    /// @tparam T Component type
    /// @param components Result components
    template <Concepts::ComponentT T>
    void GetComponents(std::vector<T*>* components);

    /// @brief Gets all of the specified Component
    /// @tparam T Component type
    /// @param components Result components
    template <Concepts::ComponentT T>
    void GetComponents(std::vector<const T*>* components) const;

    /// @brief Tries to get a component
    /// @tparam T Component type
    /// @param output Found Component
    /// @return Whether the Component exists
    template <Concepts::ComponentT T>
    [[nodiscard]]
    bool_t TryGetComponent(T** output);

    /// @brief Tries to get a component
    /// @tparam T Component type
    /// @param output Found Component
    /// @return Whether the Component exists
    template <Concepts::ComponentT T>
    [[nodiscard]]
    bool_t TryGetComponent(const T** output) const;

    /// @brief Removes a specified Component
    /// @tparam T Component type
    template <Concepts::ComponentT T>
    void RemoveComponent();

    /// @brief Removes a specified Component
    /// @param component Component instance
    XNOR_ENGINE void RemoveComponent(const Component* component);

    /// @brief Gets the Guid of the entity
    /// @return Guid
    [[nodiscard]]
    XNOR_ENGINE const Guid& GetGuid() const;

    /// @brief Gets the parent of the entity
    /// @return Parent, can be @c nullptr
    [[nodiscard]]
    XNOR_ENGINE Entity* GetParent() const;

    /// @brief Gets whether the entity has a parent
    /// @return Has parent
    [[nodiscard]]
    XNOR_ENGINE bool_t HasParent() const;

    /// @brief Gets a child at a given index
    /// @param index Index
    /// @return Child
    [[nodiscard]]
    XNOR_ENGINE Entity* GetChild(size_t index) const;

    /// @brief Gets the number of children of this entity
    /// @return Children count
    [[nodiscard]]
    XNOR_ENGINE size_t GetChildCount() const;

    /// @brief Gets whether this entity has children
    /// @return Has children
    [[nodiscard]]
    XNOR_ENGINE bool_t HasChildren() const;

    /// @brief Checks if the entity is the parent of the provided entity
    /// @param child Child entity
    /// @return If this is any parent of child
    [[nodiscard]]
    XNOR_ENGINE bool_t IsParentOf(const Entity* child) const;

    /// @brief Sets the parent of the entity
    /// @param parent New parent, use @c nullptr to orphan it
    XNOR_ENGINE void SetParent(Entity* parent);

    /// @brief Adds a child to the entity
    /// @param child Child
    XNOR_ENGINE void AddChild(Entity* child);

    /// @brief Removes a child from the entity
    /// @param child Child
    XNOR_ENGINE void RemoveChild(Entity* child);

    /// @brief Awake the entity
    XNOR_ENGINE void Awake();
    
    /// @brief Begins behavior for the entity
    XNOR_ENGINE void Begin();

    /// @brief Updates the entity
    XNOR_ENGINE void Update();

    /// @brief Called before the physics update
    XNOR_ENGINE void PrePhysics();

    /// @brief Called after the physics update
    XNOR_ENGINE void PostPhysics();

    XNOR_ENGINE void OnRendering();

    /// @brief Clones the current Entity.
    ///
    /// This function creates a new Entity at the same scope in the scene graph and with the same
    /// components, parent, and children, but a different ID.
    ///
    /// Be aware that this function can be very performance heavy, especially for Entities with a lot
    /// of nested children.
    XNOR_ENGINE Entity* Clone() const;

    XNOR_ENGINE void LookAt(const Vector3& sourcePoint, const Vector3& at);

    /// @brief Compares 2 entities using their Guid
    /// @param entity Other
    /// @return Equals
    XNOR_ENGINE bool_t operator==(const Entity& entity) const;
    

#ifdef SWIG_ONLY
    const List<Component*>& GetComponents() const;
#endif
    
private:
    XNOR_ENGINE explicit Entity(const Guid& entiyId);
    
    Entity* m_Parent = nullptr;
    
    List<Entity*> m_Children;
    
    Guid m_EntityId = Guid::New();

    List<Component*> m_Components;

    friend class Scene;
    friend class Component;
};

END_XNOR_CORE

#include "scene/entity.inl"

REFL_AUTO(
    type(XnorCore::Entity),
    field(name),
    field(m_EntityId, XnorCore::Reflection::HideInInspector(), XnorCore::Reflection::DontClone()),
    field(transform),
    field(m_Components),
    field(m_Parent, XnorCore::Reflection::HideInInspector(), XnorCore::Reflection::DontClone()),
    field(m_Children, XnorCore::Reflection::HideInInspector(), XnorCore::Reflection::DontClone())
)
