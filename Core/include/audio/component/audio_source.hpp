#pragma once

#include "core.hpp"
#include "resource/audio_track.hpp"
#include "scene/component.hpp"

BEGIN_XNOR_CORE

class AudioSource : public Component
{
    REFLECTABLE_IMPL(AudioSource)
    
public:
    Pointer<AudioTrack> audioTrack;
    
    XNOR_ENGINE AudioSource();
    
    XNOR_ENGINE ~AudioSource() override;

    DEFAULT_COPY_MOVE_OPERATIONS(AudioSource)
    
    XNOR_ENGINE void Begin() override;
    
    XNOR_ENGINE void Update() override;

    XNOR_ENGINE void Play();

    XNOR_ENGINE void Play(AudioTrack& track);

    XNOR_ENGINE void SetBuffer(const AudioBuffer* buffer);

    XNOR_ENGINE float_t GetVolume() const;

    XNOR_ENGINE void SetVolume(float_t newVolume);

    XNOR_ENGINE float_t GetPitch() const;

    XNOR_ENGINE void SetPitch(float_t newPitch);

    XNOR_ENGINE bool_t GetLooping() const;

    XNOR_ENGINE void SetLooping(bool_t newLooping);

private:
    uint32_t m_Handle = 0;
    
    /// @brief Volume in the range [0, inf]. Default value is 1.
    float_t m_Volume = 1.f;
    
    /// @brief Pitch in the range [0, inf]. Default value is 1.
    float_t m_Pitch = 1.f;

    bool_t m_Looping = false;

    bool_t m_PlayOnBegin = true;
};

END_XNOR_CORE

REFL_AUTO(
    type(XnorCore::AudioSource, bases<XnorCore::Component>),
    field(
        audioTrack,
        XnorCore::Reflection::DontExpand()
    ),
    field(
        m_Volume,
        XnorCore::Reflection::Range(0.f, 5.f),
        XnorCore::Reflection::ModifiedCallback<XnorCore::AudioSource>(
            [](XnorCore::AudioSource* source)
            {
                source->SetVolume(source->m_Volume);
            }
        )
    ),
    field(
        m_Pitch,
        XnorCore::Reflection::Range(0.f, 5.f),
        XnorCore::Reflection::ModifiedCallback<XnorCore::AudioSource>(
            [](XnorCore::AudioSource* source)
            {
                source->SetPitch(source->m_Pitch);
            }
        )
    ),
    field(
        m_Looping,
        XnorCore::Reflection::ModifiedCallback<XnorCore::AudioSource>(
            [](XnorCore::AudioSource* source)
            {
                source->SetLooping(source->m_Looping);
            }
        )
    ),
    field(m_PlayOnBegin)
)
