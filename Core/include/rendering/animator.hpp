#pragma once

#include "core.hpp"
#include "rhi_typedef.hpp"
#include "Maths/matrix.hpp"
#include "reflection/reflection.hpp"
#include "utils/list.hpp"
#include "utils/pointer.hpp"

BEGIN_XNOR_CORE

class Animation;

class Animator final
{
    REFLECTABLE_IMPL(Animator)

public:
    XNOR_ENGINE Animator() = default;
    XNOR_ENGINE explicit Animator(const Pointer<Animation>& animation);
    XNOR_ENGINE ~Animator() = default;

    DEFAULT_COPY_MOVE_OPERATIONS(Animator)

    XNOR_ENGINE void Start(const Pointer<Animation>& animation);
    XNOR_ENGINE void StartBlending(Animator* target);

    XNOR_ENGINE void Animate();

    XNOR_ENGINE void SetCrossFadeDelta(float_t delta);

    [[nodiscard]]
    XNOR_ENGINE const List<Matrix>& GetMatrices() const;
    
private:
    XNOR_ENGINE void UpdateTime();
    
    Pointer<Animation> m_Animation;
    
    float_t m_Time = 0.f;
    
    size_t m_CurrentFrame = 0;
    
    float_t m_PlaySpeed = 1.f;

    size_t m_FrameCount;

    mutable List<Matrix> m_FinalMatrices = List<Matrix>(MaxBones);
    
    List<Vector3> m_Positions = List<Vector3>(MaxBones);
    
    List<Quaternion> m_Rotations = List<Quaternion>(MaxBones);

    float_t m_CrossFadeT = 0.f;
    
    Animator* m_BlendTarget = nullptr;

    bool_t m_IsFinished = false;
};

END_XNOR_CORE

REFL_AUTO(type(XnorCore::Animator),
    field(m_Animation),
    field(m_CurrentFrame, XnorCore::Reflection::DynamicRange(&XnorCore::Animator::m_FrameCount)),
    field(m_PlaySpeed, XnorCore::Reflection::Range(-10.f, 10.f)),
    field(m_CrossFadeT, XnorCore::Reflection::Range(0.f, 1.f))
);
