#include "scene/component/skinned_mesh_renderer.hpp"

#include "rendering/animator.hpp"

using namespace XnorCore;

void SkinnedMeshRenderer::Begin()
{
}

void SkinnedMeshRenderer::OnRendering()
{
    if (m_CurrentMontage)
    {
        m_CurrentMontage->Update(this);
    }

    m_Animator.Animate();
}

void SkinnedMeshRenderer::StartAnimation(const Pointer<Animation>& animation)
{
    m_Animator.Start(animation);
}

void SkinnedMeshRenderer::StartBlending(const Pointer<Animation>& animation)
{
    m_TargetAnimator.Start(animation);
    m_Animator.StartBlending(&m_TargetAnimator);
}

void SkinnedMeshRenderer::SetCrossFadeDelta(const float_t delta)
{
    m_Animator.SetCrossFadeDelta(delta);
}

void SkinnedMeshRenderer::StartMontage(AnimationMontage* const montage)
{
    m_CurrentMontage = montage;
    m_CurrentMontage->Start();
}

const List<Matrix>& SkinnedMeshRenderer::GetMatrices() const
{
    return m_Animator.GetMatrices();
}
