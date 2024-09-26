#include "resource/audio_track.hpp"

#include "audio/audio.hpp"
#include "utils/logger.hpp"

using namespace XnorCore;

AudioTrack::~AudioTrack()
{
    AudioTrack::Unload();
}

bool_t AudioTrack::Load(const uint8_t* const buffer, const int64_t length)
{
    const char_t* const type = reinterpret_cast<const char_t*>(buffer);

    if (std::strncmp(type, "RIFF", 4) == 0)
        return LoadWavefront(buffer, length);

    Logger::LogError("Unsupported file format {}", std::string_view(type, 4));
    return false;
}

void AudioTrack::CreateInInterface()
{
    m_Buffer = new AudioBuffer(this);
    Audio::RegisterBuffer(m_Buffer);
    
    m_LoadedInInterface = true;
}

void AudioTrack::DestroyInInterface()
{
    Audio::UnregisterBuffer(m_Buffer);
    delete m_Buffer;
    m_Buffer = nullptr;
    
    m_LoadedInInterface = false;
}

void AudioTrack::Unload()
{
    // The data was new-ed by the File wrapper, so we don't need to delete it here
    m_Data = nullptr;
    m_DataSize = 0;
    m_Channels = 0;
    m_SampleRate = 0;
    m_BitDepth = 0;
    
    m_Loaded = false;
}

int32_t AudioTrack::GetDataSize() const
{
    return m_DataSize;
}

uint16_t AudioTrack::GetChannels() const
{
    return m_Channels;
}

int32_t AudioTrack::GetSampleRate() const
{
    return m_SampleRate;
}

uint16_t AudioTrack::GetBitDepth() const
{
    return m_BitDepth;
}

const AudioBuffer* AudioTrack::GetBuffer() const
{
    return m_Buffer;
}

bool_t AudioTrack::LoadWavefront(const uint8_t* const buffer, const int64_t length)
{
    int64_t offset = 8; // 4 because 'RIFF' has already been checked and 4 because of the file size that we don't need
    const char_t* const str = reinterpret_cast<const char_t*>(buffer);
    
    if (std::strncmp(str + offset, "WAVE", 4) != 0)
        return false;
    offset += 4;

    while (offset < length)
    {
        if (strncmp(str + offset, "fmt ", 4) == 0)
            offset += LoadWavefrontFormat(buffer + offset);
        else if (strncmp(str + offset, "data", 4) == 0)
            offset += LoadWavefrontData(buffer + offset);
        else
            offset++;
    }

    m_Loaded = true;
    return true;
}

int64_t AudioTrack::LoadWavefrontFormat(const uint8_t* const data)
{
    int64_t offset = 8; // 4 because 'fmt ' has already been checked and 4 because of the chunk size that we don't need
    
    offset += sizeof(uint16_t); // Skip the format type - we don't care about it
    
    m_Channels = *reinterpret_cast<const decltype(m_Channels)*>(data + offset);
    offset += sizeof(m_Channels);
    
    m_SampleRate = *reinterpret_cast<const decltype(m_SampleRate)*>(data + offset);
    offset += sizeof(m_SampleRate);

    offset += sizeof(uint32_t); // Skip whatever is this value

    offset += sizeof(uint16_t); // As well as this one
    
    m_BitDepth = *reinterpret_cast<const decltype(m_BitDepth)*>(data + offset);
    offset += sizeof(m_BitDepth);

    return offset;
}

int64_t AudioTrack::LoadWavefrontData(const uint8_t* const data)
{
    const int32_t length = *reinterpret_cast<const int32_t*>(data + 4);
    m_Data = data + 8;
    m_DataSize = length;

    return length + 8;
}
