#pragma once

#include <ImGui/imgui.h>

#include "core.hpp"
#include "scene/entity.hpp"

/// @file filters.hpp
/// @brief Defines the Filters class.

BEGIN_XNOR_CORE

/// @brief Provides utility functions to perform filters for the UI of the engine
class Filters
{
    STATIC_CLASS(Filters)

public:
    /// @brief Handles the resource filter
    /// @tparam T Resource type
    /// @param target Target resource
    /// @return Selected value
    template <Concepts::ResourceT T>
    static Pointer<T> FilterResources(Pointer<T>* target);

    /// @brief Handles the resource filter
    /// @param target Pointer<File>* target
    /// @return Pointer<File>* return value
    XNOR_ENGINE static Pointer<File> FilterFile(Pointer<File>* target);

    /// @brief Handles the entity filter
    /// @param target Target entity
    /// @return Selected value
    XNOR_ENGINE static Entity* FilterEntity(Entity** target);

    /// @brief Handles the entity filter
    /// @param target Target entity
    /// @return Selected value
    XNOR_ENGINE static Entity* FilterEntity();

    /// @brief Handles the component filter (with a List)
    /// @param target Target list
    /// @return Selected value
    XNOR_ENGINE static Component* FilterComponent(List<Component*>* target);

    /// @brief Handles the component filter (with a raw Component)
    /// @param target Target component
    /// @return Selected value
    XNOR_ENGINE static Component* FilterComponent(Component** target);

    /// @brief Begins a resource filter
    XNOR_ENGINE static void BeginResourceFilter();
    /// @brief Begins an entity filter
    XNOR_ENGINE static void BeginEntityFilter();
    /// @brief Begins a component filter
    XNOR_ENGINE static void BeginComponentFilter();
    /// @brief Begins a filter
    XNOR_ENGINE static void BeginFilter(const std::string& filterName = "File");

private:
    static inline ImGuiTextFilter m_TextFilter;
};

END_XNOR_CORE

#include "reflection/filters.inl"
