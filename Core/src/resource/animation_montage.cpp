#include "resource/animation_montage.hpp"
#include "scene/component/skinned_mesh_renderer.hpp"

using namespace XnorCore;

void AnimationMontage::Start()
{
    m_NotifiesTimeline.Start();
    m_AnimationTimeline.Start();
    m_Ended = false;
}

void AnimationMontage::Update(SkinnedMeshRenderer* renderer)
{
    if (!m_Ended)
    {
        m_NotifiesTimeline.Update();
        m_Ended = m_AnimationTimeline.Update(std::forward<SkinnedMeshRenderer*>(renderer));

        if (m_Ended && looping)
        {
            Start();            
        }
    }
}

void AnimationMontage::AddEvent(const float_t when, const FunctionT& function)
{
    m_NotifiesTimeline.AddBeginEvent(when, function);
}

void AnimationMontage::AddAnimation(const float_t when, const size_t animationId)
{
    Pointer<Animation> animation = mesh->GetAnimation(animationId);

    if (animation == nullptr)
    {
        Logger::LogError("Animation with id {} doesn't exist", animationId);
        return;
    }

    UpdateTimelineDuration(animation->GetDuration());

    m_AnimationTimeline.SetEventDuration(when, animation->GetDuration());

    //m_AnimationTimeline.AddBeginEvent(when, [this, animation](SkinnedMeshRenderer* const renderer) -> void
    //{
      //  renderer->StartAnimation(animation);
    //});
}

void AnimationMontage::AddAnimation(const float_t when, const Pointer<Animation>& animation)
{
    UpdateTimelineDuration(animation->GetDuration());

    m_AnimationTimeline.SetEventDuration(when, animation->GetDuration());

    m_AnimationTimeline.AddBeginEvent(when, [this, animation](SkinnedMeshRenderer* const renderer) -> void
    {
        renderer->StartAnimation(animation);
    });
}

void AnimationMontage::AddAnimationBlending(const float_t when, const size_t sourceAnimation, const size_t targetAnimation, const float_t duration)
{
     const Pointer<Animation> sourceAnim = mesh->GetAnimation(sourceAnimation);
     const Pointer<Animation> targetAnim = mesh->GetAnimation(targetAnimation);

    if (sourceAnim == nullptr)
    {
        Logger::LogError("Animation with id {} doesn't exist", sourceAnimation);
        return;
    }

    if (targetAnim == nullptr)
    {
        Logger::LogError("Animation with id {} doesn't exist", targetAnimation);
        return;
    }

    UpdateTimelineDuration(duration);

    m_AnimationTimeline.SetEventDuration(when, duration);

    m_AnimationTimeline.AddBeginEvent(when, [&](SkinnedMeshRenderer* const renderer) -> void
    {
        renderer->StartAnimation(sourceAnim);
        renderer->StartBlending(targetAnim);
    });

    m_AnimationTimeline.AddUpdateEvent(when, [&](const float_t deltaTime, SkinnedMeshRenderer* const renderer) -> void
    {
        renderer->SetCrossFadeDelta(deltaTime);
    });
}

bool_t AnimationMontage::HasEnded() const
{
    return m_Ended;
}

void AnimationMontage::UpdateTimelineDuration(const float_t duration)
{
    m_AnimationTimeline.SetDuration(m_AnimationTimeline.GetDuration() + duration);
    m_NotifiesTimeline.SetDuration(m_NotifiesTimeline.GetDuration() + duration);
}
