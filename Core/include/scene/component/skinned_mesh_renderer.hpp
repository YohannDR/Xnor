#pragma once

#include "core.hpp"
#include "rendering/animator.hpp"
#include "rendering/material.hpp"
#include "resource/animation_montage.hpp"
#include "resource/mesh.hpp"
#include "resource/model.hpp"
#include "scene/component.hpp"

/// @file skinned_mesh_renderer.hpp
/// @brief Defines the XnorCore::SkinnedMeshRender class.

BEGIN_XNOR_CORE

/// @brief Component that defines a mesh to render
class SkinnedMeshRenderer final : public Component
{
    REFLECTABLE_IMPL(SkinnedMeshRenderer)

public:
    XNOR_ENGINE SkinnedMeshRenderer() = default;

    XNOR_ENGINE void Begin() override;
    
    XNOR_ENGINE void OnRendering() override;

    /// @brief @ref Mesh
    Pointer<Mesh> mesh;

    Material material;

    /// @brief Whether to draw the model AABB box
    bool_t drawModelAabb = false;

    XNOR_ENGINE void StartAnimation(const Pointer<Animation>& animation);
    XNOR_ENGINE void StartBlending(const Pointer<Animation>& animation);
    XNOR_ENGINE void SetCrossFadeDelta(float_t delta);

    XNOR_ENGINE void StartMontage(AnimationMontage* montage);

    XNOR_ENGINE const List<Matrix>& GetMatrices() const;

private:
    Animator m_Animator;
    Animator m_TargetAnimator;

    AnimationMontage* m_CurrentMontage;
};

END_XNOR_CORE

/// @private
REFL_AUTO(
    type(XnorCore::SkinnedMeshRenderer, bases<XnorCore::Component>),
    field(mesh),
    field(material),
    field(drawModelAabb),
    field(m_Animator)
);
