﻿#include "audio/audio_buffer.hpp"

#include <AL/al.h>

#include "AL/alext.h"
#include "audio/audio.hpp"
#include "resource/audio_track.hpp"
#include "utils/logger.hpp"

using namespace XnorCore;

AudioBuffer::AudioBuffer()
    : m_Context(Audio::GetContext())
{
    m_Context->MakeCurrent();
    alGenBuffers(1, &m_Handle);
}

AudioBuffer::AudioBuffer(const int32_t size)
    : AudioBuffer()
{
    alBufferi(m_Handle, AL_SIZE, size);
}

AudioBuffer::AudioBuffer(const AudioTrack* track)
    : AudioBuffer()
{
    SetData(track);
}

AudioBuffer::~AudioBuffer()
{
    m_Context->MakeCurrent();
    alDeleteBuffers(1, &m_Handle);
}

void AudioBuffer::SetData(const AudioTrack* track)
{
    SetData(track->GetData(), track->GetDataSize(), track->GetChannels(), track->GetBitDepth(), track->GetSampleRate());
}

// ReSharper disable once CppMemberFunctionMayBeConst
void AudioBuffer::SetData(const void* const data, const int32_t length, const uint16_t channels, const uint16_t bitDepth, const int32_t sampleRate)
{
    m_Context->MakeCurrent();
    alBufferData(m_Handle, AlFormatFromData(channels, bitDepth), data, length, sampleRate);
    AudioContext::CheckError();
}

uint32_t AudioBuffer::GetHandle() const
{
    return m_Handle;
}

int32_t AudioBuffer::AlFormatFromData(const uint16_t channels, const uint16_t bitDepth)
{
    ALenum format = 0;
    if (channels == 1)
    {
        switch (bitDepth)
        {
            case 8:
                format = AL_FORMAT_MONO8;
                break;
            case 16:
                format = AL_FORMAT_MONO16;
                break;
            case 32:
                format = AL_FORMAT_MONO_FLOAT32;
                break;
            case 64:
                format = AL_FORMAT_MONO_DOUBLE_EXT;
                break;
            default: ;
        }
    }
    else if (channels == 2)
    {
        switch (bitDepth)
        {
            case 8:
                format = AL_FORMAT_STEREO8;
                break;
            case 16:
                format = AL_FORMAT_STEREO16;
                break;
            case 32:
                format = AL_FORMAT_STEREO_FLOAT32;
                break;
            case 64:
                format = AL_FORMAT_STEREO_DOUBLE_EXT;
                break;
            default: ;
        }
    }

    if (format == 0)
        Logger::LogError("Unsupported number of channels ({}) or bits ({}) for AudioBuffer", channels, bitDepth);

    return format;
}
