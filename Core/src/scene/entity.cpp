#include "scene/entity.hpp"

#include "scene/component.hpp"
#include "scene/component/script_component.hpp"
#include "serialization/serializer.hpp"
#include "utils/logger.hpp"
#include "world/scene_graph.hpp"

using namespace XnorCore;

Entity::Entity(const Guid& entiyId)
    : m_EntityId(entiyId)
{
    SceneGraph::UpdateTransform(*this);
}

Entity::~Entity()
{
    decltype(m_Components) copy(m_Components);

    for (size_t i = 0; i < copy.GetSize(); i++)
        copy[i]->Destroy();
}

void Entity::Begin()
{
    for (size_t i = 0; i < m_Components.GetSize(); i++)
    {
        if (!m_Components[i]->enabled)
            continue;
            
        m_Components[i]->Begin();
    }
}

void Entity::Update()
{
    for (size_t i = 0; i < m_Components.GetSize(); i++)
    {
        if (!m_Components[i]->enabled)
            continue;
        
        m_Components[i]->Update();
    }
}

void Entity::PrePhysics()
{
    for (size_t i = 0; i < m_Components.GetSize(); i++)
    {
        m_Components[i]->PrePhysics();
    }
}

void Entity::PostPhysics()
{
    for (size_t i = 0; i < m_Components.GetSize(); i++)
    {
        m_Components[i]->PostPhysics();
    }
}

void Entity::OnRendering()
{
    for (size_t i = 0; i < m_Components.GetSize(); i++)
        m_Components[i]->OnRendering();
}

Entity* Entity::Clone() const
{
    Entity* clone = World::scene->CreateEntity(name, nullptr);
    Reflection::Clone<Entity>(this, clone);

    for (const Entity* child : m_Children)
        clone->AddChild(child->Clone());

    return clone;
}

void Entity::LookAt(const Vector3& sourcePoint, const Vector3& at)
{
    const Vector3 forwardVector = (at -  sourcePoint).Normalized();

    const Vector3 rotAxis = Vector3::Cross(Vector3::UnitZ(), forwardVector);
    const float_t dot = Vector3::Dot(Vector3::UnitZ(), forwardVector);

    const Quaternion q = { rotAxis.x, rotAxis.y, rotAxis.z, dot + 1.f};

    transform.SetRotation(q.Normalized());
}

void Entity::AddComponent(Component* const component)
{
    component->entity = this;
    m_Components.Add(component);
    
    if (World::isPlaying)
    {
        component->Awake();
        component->Begin();
    }
}

void Entity::RemoveComponent(const Component* const component)
{
    for (size_t i = 0; i < m_Components.GetSize(); i++)
    {
        if (m_Components[i] == component)
        {
            m_Components.RemoveAt(i);
            break;
        }
    }
}

const Guid& Entity::GetGuid() const
{
    return m_EntityId;
}

Entity* Entity::GetParent() const
{
    return m_Parent;
}

bool_t Entity::HasParent() const
{
    return m_Parent != nullptr;
}

Entity* Entity::GetChild(const size_t index) const
{
    return m_Children[index];
}

size_t Entity::GetChildCount() const
{
    return m_Children.GetSize();
}

bool_t Entity::HasChildren() const
{
    return GetChildCount() != 0;
}

bool_t Entity::IsParentOf(const Entity* child) const
{
    const Entity* e = child->m_Parent;

    while (e)
    {
        if (e == this)
            return true;

        e = e->m_Parent;
    }

    return false;
}

void Entity::SetParent(Entity* const parent)
{
    // Remove ourselves from our old parent if we had one
    if (HasParent())
        m_Parent->m_Children.Remove(this);

    // Set new parent
    m_Parent = parent;

    // Need to check if we actually have a parent, since a nullptr parent is valid
    if (parent)
    {
        SceneGraph::OnAttachToParent(*this);
        // Add ourselves to our new parent
        parent->m_Children.Add(this);
    }
}

void Entity::AddChild(Entity* child)
{
    if (child == nullptr)
    {
        Logger::LogWarning("Trying to add a child that doesn't exists");
        return;
    }

    // Add the enw child
    m_Children.Add(child);

    // Check if the child add a parent
    if (child->HasParent())
    {
        // If it had one, remove its old child affiliation
        child->m_Parent->m_Children.Remove(this);
    }

    // Set the new parent of the child to ourselves
    child->m_Parent = this;
}

void Entity::RemoveChild(Entity* const child)
{
    if (child == nullptr)
    {
        Logger::LogWarning("Trying to remove a child that doesn't exists");
        return;
    }
    
    // Remove from the list if the parent was indeed us
    if (child->m_Parent == this)
        m_Children.Remove(child);

    // Orphan the child
    child->m_Parent = nullptr;
}

void Entity::Awake()
{
    for (size_t i = 0; i < m_Components.GetSize(); i++)
    {
        m_Components[i]->Awake();
    }
}

bool_t Entity::operator==(const Entity& entity) const
{
    return m_EntityId == entity.m_EntityId;
}
