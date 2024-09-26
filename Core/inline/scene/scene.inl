#pragma once

BEGIN_XNOR_CORE

template <class ComponentT>
void Scene::GetAllComponentsOfType(std::vector<const ComponentT*>* const components) const
{
    components->clear();
    
    for (size_t i = 0; i < m_Entities.GetSize(); i++)
    {
        std::vector<ComponentT*> entityComponents;
        m_Entities[i]->GetComponents<ComponentT>(&entityComponents);

        if (!entityComponents.empty())
            components->insert(components->end(), entityComponents.cbegin(), entityComponents.cend());
    }
}

template <class ComponentT>
void Scene::GetAllComponentsOfType(std::vector<ComponentT*>* const components)
{
    components->clear();
    
    for (size_t i = 0; i < m_Entities.GetSize(); i++)
    {
        std::vector<ComponentT*> entityComponents;
        m_Entities[i]->GetComponents<ComponentT>(&entityComponents);

        if (!entityComponents.empty())
            components->insert(components->end(), entityComponents.cbegin(), entityComponents.cend());
    }
}

END_XNOR_CORE
