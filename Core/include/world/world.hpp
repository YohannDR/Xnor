#pragma once

#include "core.hpp"
#include "skybox.hpp"
#include "scene/scene.hpp"

/// @file world.hpp
/// @brief Defines the XnorCore::World class.

BEGIN_XNOR_CORE

/// @brief Represents a world, it's a superset of a scene and encapsulates one
class World
{
    STATIC_CLASS(World)
    
public:
    /// @brief Called every frame when the world is playing
    XNOR_ENGINE static void Update();
    
    /// @brief Whether the world is playing/running
    XNOR_ENGINE static inline bool_t isPlaying = false;

    /// @brief Whether the world is playing/running
    XNOR_ENGINE static inline bool_t hasStarted = false;

    /// @brief The currently loaded scene in the world
    XNOR_ENGINE static inline Scene* scene;
};

END_XNOR_CORE
