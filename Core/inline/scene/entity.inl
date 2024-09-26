#pragma once
#include "scene/component/script_component.hpp"

BEGIN_XNOR_CORE
    template <Concepts::ComponentT T>
T* Entity::AddComponent()
{
    T* newT = new T;
    AddComponent(newT);
    return newT;
}

template <Concepts::ComponentT T>
const T* Entity::GetComponent() const
{
    for (size_t i = 0; i < m_Components.GetSize(); i++)
    {
        if (dynamic_cast<T*>(m_Components[i]))
            return reinterpret_cast<T*>(m_Components[i]);
    }

    return nullptr;
}

template <Concepts::ComponentT T>
void Entity::GetComponents(std::vector<T*>* components)
{
    for (size_t i = 0; i < m_Components.GetSize(); i++)
    {
        if (dynamic_cast<T*>(m_Components[i]))
            components->push_back(reinterpret_cast<T*>(m_Components[i]));
    }
}

template <Concepts::ComponentT T>
void Entity::GetComponents(std::vector<const T*>* components) const
{
    for (size_t i = 0; i < m_Components.GetSize(); i++)
    {
        if (dynamic_cast<T*>(m_Components[i]))
            components->push_back(reinterpret_cast<const T*>(m_Components[i]));
    }
}

template <Concepts::ComponentT T>
T* Entity::GetComponent()
{
    for (size_t i = 0; i < m_Components.GetSize(); i++)
    {
        if (dynamic_cast<T*>(m_Components[i]))
            return reinterpret_cast<T*>(m_Components[i]);
    }

    return nullptr;
}

template <Concepts::ComponentT T>
void Entity::RemoveComponent()
{
    for (int i = 0; i < m_Components.GetSize(); i++)
    {
        if (dynamic_cast<T*>(m_Components[i]))
        {
            m_Components[i]->Destroy();
            m_Components.RemoveAt(i);
            break;
        }
    }
}

template <Concepts::ComponentT T>
bool_t Entity::TryGetComponent(T** const output)
{
    for (int i = 0; i < m_Components.GetSize(); i++)
    {
        if (dynamic_cast<T*>(m_Components[i]))
        {
            *output = reinterpret_cast<T*>(m_Components[i]);
            return true;
        }
    }
    
    return false;
}

template <Concepts::ComponentT T>
bool_t Entity::TryGetComponent(const T** const output) const
{
    for (int i = 0; i < m_Components.GetSize(); i++)
    {
        if (dynamic_cast<T*>(m_Components[i]))
        {
            *output = reinterpret_cast<T*>(m_Components[i]);
            return true;
        }
    }
    
    return false;
}

#ifdef SWIG_ONLY
const List<Component*>& Entity::GetComponents() const
{
    return m_Components;
}
#endif

END_XNOR_CORE
