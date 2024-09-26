#pragma once

#include "core.hpp"
#include "scene/entity.hpp"

/// @file scene_graph.hpp
/// @brief Defines the XnorCore::SceneGraph class

BEGIN_XNOR_CORE

/// @brief Provides functions to handle parent/child entity transformation hierarchy
class SceneGraph
{
    STATIC_CLASS(SceneGraph)
    
public:
    /// @brief Updates an entity when its parent changed
    /// @param entity Entity
    XNOR_ENGINE static void OnAttachToParent(Entity& entity);
    
    /// @brief Updates the transformation of a list of entities
    /// @param entities List of entities
    XNOR_ENGINE static void Update(const List<Entity*>& entities);

    /// @brief Updates the Transform of the given Entity.
    XNOR_ENGINE static void UpdateTransform(Entity& entity);
};

END_XNOR_CORE
