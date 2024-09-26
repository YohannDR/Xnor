#pragma once

#include <array>

#include "core.hpp"
#include "skeleton.hpp"
#include "assimp/scene.h"
#include "file/file.hpp"
#include "rendering/bone.hpp"
#include "rendering/rhi_typedef.hpp"
#include "resource/resource.hpp"
#include "utils/list.hpp"

BEGIN_XNOR_CORE

class Animation final : public Resource
{
    REFLECTABLE_IMPL(Animation)
    
public:
    struct KeyFrame
    {
        Vector3 translation;
        Quaternion rotation;
        Vector3 scaling;
        float_t time{};
    };

    Pointer<Skeleton> skeleton;

    /// @brief Allowed extensions for animations.
    XNOR_ENGINE static inline constexpr std::array<const char_t*, 0> FileExtensions
    {
    };

    // Use the base class' constructors
    using Resource::Resource;

    // We keep both function overloads and only override one
    using Resource::Load;
    
    DEFAULT_COPY_MOVE_OPERATIONS(Animation)

    XNOR_ENGINE Animation() = default;

    XNOR_ENGINE ~Animation() override = default;

    XNOR_ENGINE void BindSkeleton(Pointer<Skeleton> bindedSkeleton);

    /// @copydoc XnorCore::Resource::Load(const uint8_t* buffer, int64_t length)
    XNOR_ENGINE bool_t Load(const uint8_t* buffer, int64_t length) override;

    XNOR_ENGINE bool_t Load(const aiAnimation& loadedData);

    [[nodiscard]]
    XNOR_ENGINE float_t GetDuration() const;

    [[nodiscard]]
    XNOR_ENGINE size_t GetFrameCount() const;
    
    [[nodiscard]]
    XNOR_ENGINE float_t GetFramerate() const;
    
    [[nodiscard]]
    XNOR_ENGINE float_t GetFrameDuration() const;

    XNOR_ENGINE void GetBoneKeyFrame(const Bone& bone, const List<Animation::KeyFrame>** keyFrames) const;

private:
    float_t m_Duration;
    float_t m_Framerate;
    float_t m_FrameDuration;
    size_t m_FrameCount;
    std::unordered_map<std::string, List<KeyFrame>> m_KeyFrames;

};

END_XNOR_CORE

REFL_AUTO(type(XnorCore::Animation, bases<XnorCore::Resource>),
    field(skeleton),
    field(m_Duration, XnorCore::Reflection::ReadOnly()),
    field(m_Framerate, XnorCore::Reflection::ReadOnly())
)
