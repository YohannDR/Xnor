#include "audio/component/audio_listener.hpp"

#include <AL/al.h>

#include "transform.hpp"
#include "audio/audio.hpp"
#include "input/time.hpp"

using namespace XnorCore;

void AudioListener::Update()
{
    Audio::GetContext()->MakeCurrent();

    const Transform& transform = GetTransform();

    const Vector3& position = transform.GetPosition();

    // Position
    alListenerfv(AL_POSITION, position.Raw());
    AudioContext::CheckError();

    Vector3 velocity = (position - m_LastPosition) / Time::GetDeltaTime();
    std::array orientation = { transform.worldMatrix * Vector3::UnitZ(), Vector3::UnitY() };
    if (!dopplerEffect)
    {
        velocity = Vector3::Zero();
        orientation[0] = Vector3::Zero();
    }
    
    // Velocity
    alListenerfv(AL_VELOCITY, velocity.Raw());
    AudioContext::CheckError();

    // Orientation
    alListenerfv(AL_ORIENTATION, orientation[0].Raw());
    AudioContext::CheckError();

    m_LastPosition = position;
}

float_t AudioListener::GetVolume() const
{
    return m_Volume;
}

void AudioListener::SetVolume(const float_t newVolume)
{
    m_Volume = std::max(0.f, newVolume);
    
    Audio::GetContext()->MakeCurrent();
    alListenerf(AL_GAIN, m_Volume);
    AudioContext::CheckError();
}
