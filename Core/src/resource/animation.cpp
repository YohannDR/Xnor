#include "resource/animation.hpp"

#include "input/time.hpp"
#include "rendering/animator.hpp"
#include "rendering/rhi_typedef.hpp"
#include "utils/logger.hpp"

using namespace XnorCore;

void Animation::BindSkeleton(Pointer<Skeleton> bindedSkeleton)
{
    skeleton = std::move(bindedSkeleton);
}

bool_t Animation::Load(const uint8_t* const, const int64_t)
{
    return false;
}

bool_t Animation::Load(const aiAnimation& loadedData)
{
    m_FrameCount = static_cast<size_t>(loadedData.mDuration);
    m_Framerate = static_cast<float_t>(loadedData.mTicksPerSecond);
    m_FrameDuration = 1.f / m_Framerate;
    m_Duration = static_cast<float_t>(loadedData.mDuration / loadedData.mTicksPerSecond);

    for (uint32_t i = 0; i < loadedData.mNumChannels; i++)
    {
        const aiNodeAnim* const channel = loadedData.mChannels[i];
        std::string name = channel->mNodeName.C_Str();

        auto&& it = m_KeyFrames.emplace(name, channel->mNumPositionKeys);

        for (uint32_t j = 0; j < channel->mNumPositionKeys; j++)
        {
            const Vector3 translation = Vector3(&channel->mPositionKeys[j].mValue.x);
            const Quaternion rotation = Quaternion(Vector3(&channel->mRotationKeys[j].mValue.x), channel->mRotationKeys[j].mValue.w);
            const Vector3 scaling = Vector3(&channel->mScalingKeys[j].mValue.x);

            const KeyFrame keyFrame =
            {
                .translation = translation,
                .rotation = rotation,
                .scaling = scaling,
                .time = static_cast<float_t>(channel->mPositionKeys[j].mTime)
            };

            it.first->second[j] = keyFrame;
        }
    }

    return true;
}


float_t Animation::GetDuration() const
{
    return m_Duration;
}

size_t Animation::GetFrameCount() const
{
    return m_FrameCount;
}

float_t Animation::GetFramerate() const
{
    return m_Framerate;
}

float_t Animation::GetFrameDuration() const
{
    return m_FrameDuration;
}

void Animation::GetBoneKeyFrame(const Bone& bone, const List<Animation::KeyFrame>** keyFrames) const
{
    auto&& it = m_KeyFrames.find(bone.name);
    if (it == m_KeyFrames.end())
    {
        *keyFrames = nullptr;
        return;
    }

    *keyFrames = &it->second;
}
