#include "audio/component/audio_source.hpp"

#include <AL/al.h>

#include "transform.hpp"
#include "audio/audio.hpp"
#include "input/input.hpp"
#include "utils/logger.hpp"
#include "world/world.hpp"

using namespace XnorCore;

AudioSource::AudioSource()
{
    Audio::GetContext()->MakeCurrent();
    alGenSources(1, &m_Handle);
    AudioContext::CheckError();
    SetLooping(true);
}

AudioSource::~AudioSource()
{
    Audio::GetContext()->MakeCurrent();
    alDeleteSources(1, &m_Handle);
    AudioContext::CheckError();
}

void AudioSource::Begin()
{
    // Update pitch and volume in OpenAL
    SetVolume(m_Volume);
    SetPitch(m_Pitch);

    if (m_PlayOnBegin)
        Play();
}

void AudioSource::Update()
{
    Audio::GetContext()->MakeCurrent();

    const Transform& transform = GetTransform();
    
    // Position
    alSourcefv(m_Handle, AL_POSITION, transform.GetPosition().Raw());
    AudioContext::CheckError();

    Vector3 velocity;
    std::array orientation = { transform.worldMatrix * Vector3::UnitZ(), Vector3::UnitY() };

    orientation[0] = Vector3::Zero();
    
    // Velocity
    alSourcefv(m_Handle, AL_VELOCITY, velocity.Raw());
    AudioContext::CheckError();

    // Direction
    alSourcefv(m_Handle, AL_DIRECTION, orientation[0].Raw());
    AudioContext::CheckError();
}

void AudioSource::Play() { if (audioTrack) Play(*audioTrack); }

void AudioSource::Play(AudioTrack& track)
{
    Audio::GetContext()->MakeCurrent();
    
    if (!track.IsLoadedInInterface())
    {
        Logger::LogWarning("Tried to play an AudioTrack ({}) that wasn't loaded in the audio interface. Loading it", track.GetName());
        track.CreateInInterface();
    }

    alSourceRewind(m_Handle);
    SetBuffer(track.GetBuffer());
    alSourcePlay(m_Handle);
    AudioContext::CheckError();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void AudioSource::SetBuffer(const AudioBuffer* buffer)
{
    Audio::GetContext()->MakeCurrent();
    alSourcei(m_Handle, AL_BUFFER, static_cast<int32_t>(buffer->GetHandle()));
    AudioContext::CheckError();
}

float_t AudioSource::GetVolume() const
{
    return m_Volume;
}

void AudioSource::SetVolume(const float_t newVolume)
{
    m_Volume = std::max(0.f, newVolume);
    
    Audio::GetContext()->MakeCurrent();
    alSourcef(m_Handle, AL_GAIN, m_Volume);
    AudioContext::CheckError();
}

float_t AudioSource::GetPitch() const
{
    return m_Pitch;
}

void AudioSource::SetPitch(const float_t newPitch)
{
    m_Pitch = std::max(0.f, newPitch);
    
    Audio::GetContext()->MakeCurrent();
    alSourcef(m_Handle, AL_PITCH, m_Pitch);
    AudioContext::CheckError();
}

bool_t AudioSource::GetLooping() const
{
    return m_Looping;
}

void AudioSource::SetLooping(const bool_t newLooping)
{
    m_Looping = newLooping;
    
    Audio::GetContext()->MakeCurrent();
    alSourcei(m_Handle, AL_LOOPING, m_Looping);
    AudioContext::CheckError();
}
