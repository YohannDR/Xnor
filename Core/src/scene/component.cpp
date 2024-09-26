#include "scene/component.hpp"

#include "scene/entity.hpp"

using namespace XnorCore;

Component::~Component()
{
    if (entity)
        entity->m_Components.Remove(this);
}

void Component::Destroy()
{
    delete this;
}

const Entity* Component::GetEntity() const
{
    return entity;
}

Entity* Component::GetEntity()
{
    return entity;
}

const Transform& Component::GetTransform() const
{
    return entity->transform;
}

Transform& Component::GetTransform()
{
    return entity->transform;
}
