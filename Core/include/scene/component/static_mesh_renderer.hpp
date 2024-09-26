#pragma once

#include "core.hpp"
#include "rendering/material.hpp"
#include "resource/mesh.hpp"
#include "resource/model.hpp"
#include "scene/component.hpp"

/// @file static_mesh_renderer.hpp
/// @brief Defines the XnorCore::MeshRenderer class.

BEGIN_XNOR_CORE

/// @brief Component that defines a mesh to render
class StaticMeshRenderer : public Component
{
    REFLECTABLE_IMPL(StaticMeshRenderer)
    
public:
    /// @brief @ref Mesh
    Pointer<Mesh> mesh;

    /// @brief @ref Material to render with
    Material material;

    /// @brief Whether to draw the model AABB box
    bool_t drawModelAabb = false;
    
    XNOR_ENGINE StaticMeshRenderer() = default;

    XNOR_ENGINE ~StaticMeshRenderer() override = default;
    
    DEFAULT_COPY_MOVE_OPERATIONS(StaticMeshRenderer);

    XNOR_ENGINE void GetAabb(Bound* bound) const;
};

END_XNOR_CORE

/// @private
REFL_AUTO(
    type(XnorCore::StaticMeshRenderer, bases<XnorCore::Component>),
    field(mesh),
    field(material),
    field(drawModelAabb)
);
