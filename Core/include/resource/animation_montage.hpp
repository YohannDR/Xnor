#pragma once

#include "core.hpp"
#include "mesh.hpp"
#include "resource.hpp"
#include "rendering/animator.hpp"
#include "utils/timeline.hpp"

BEGIN_XNOR_CORE

class SkinnedMeshRenderer;

class AnimationMontage final : public Resource
{
    REFLECTABLE_IMPL(AnimationMontage)
    
    using FunctionT = Timeline<>::BeginFunctionT;

    struct AnimationInfo
    {
        size_t animationId;
        float_t duration;
        size_t animationTargetId = std::numeric_limits<size_t>::max();
        float_t blendingDuration = -1.f;
    };

public:
    /// @brief Allowed extensions for models.
    XNOR_ENGINE static inline constexpr std::array<const char_t*, 1> FileExtensions
    {
        ".am"
    };
    
    using Resource::Resource;
    XNOR_ENGINE ~AnimationMontage() override = default;

    DEFAULT_COPY_MOVE_OPERATIONS(AnimationMontage)

    XNOR_ENGINE void Start();
    XNOR_ENGINE void Update(SkinnedMeshRenderer* renderer);

    XNOR_ENGINE void AddEvent(float_t when, const FunctionT& function);
    XNOR_ENGINE void AddAnimation(float_t when, size_t animationId);
    XNOR_ENGINE void AddAnimation(float_t when, const Pointer<Animation>& animation);
    XNOR_ENGINE void AddAnimationBlending(float_t when, size_t sourceAnimation, size_t targetAnimation, float_t duration);

    [[nodiscard]]
    XNOR_ENGINE bool_t HasEnded() const;

    Pointer<Mesh> mesh;
    bool_t looping;

private:
    Timeline<> m_NotifiesTimeline;
    Timeline<SkinnedMeshRenderer*> m_AnimationTimeline;

    bool_t m_Ended;

    XNOR_ENGINE void UpdateTimelineDuration(float_t duration);
};

END_XNOR_CORE

REFL_AUTO(type(XnorCore::AnimationMontage, bases<XnorCore::Resource>, XnorCore::Reflection::OpenEditorWindow("AnimationMontageEditor")),
    field(m_NotifiesTimeline),
    field(m_AnimationTimeline),
    field(mesh)
)
