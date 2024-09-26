#pragma once

#include "core.hpp"
#include "audio/audio_buffer.hpp"
#include "resource/resource.hpp"

BEGIN_XNOR_CORE

class AudioTrack : public Resource
{
    REFLECTABLE_IMPL(AudioTrack)
    
public:
    /// @brief Allowed extensions for audio files
    XNOR_ENGINE static constexpr std::array<const char_t*, 4> FileExtensions
    {
        ".wav",
        ".wave",
        ".mp3",
        ".ogg"
    };
    
    // Same constructor from base class
    using Resource::Resource;

    XNOR_ENGINE ~AudioTrack() override;

    DEFAULT_COPY_MOVE_OPERATIONS(AudioTrack)

    // We keep both function overloads and only override one
    using Resource::Load;
    
    XNOR_ENGINE bool_t Load(const uint8_t* buffer, int64_t length) override;
    
    XNOR_ENGINE void CreateInInterface() override;
    
    XNOR_ENGINE void DestroyInInterface() override;
    
    XNOR_ENGINE void Unload() override;
    
    /// @brief Gets the raw data of the texture
    /// @tparam T Type
    /// @return Data
    template <typename T = uint8_t>
    [[nodiscard]]
    const T* GetData() const;

    /// @brief Gets the raw data of the texture
    /// @tparam T Type
    /// @return Data
    template <typename T = uint8_t>
    [[nodiscard]]
    T* GetData();
    
    [[nodiscard]]
    XNOR_ENGINE int32_t GetDataSize() const;

    [[nodiscard]]
    XNOR_ENGINE uint16_t GetChannels() const;

    [[nodiscard]]
    XNOR_ENGINE int32_t GetSampleRate() const;

    [[nodiscard]]
    XNOR_ENGINE uint16_t GetBitDepth() const;

    [[nodiscard]]
    XNOR_ENGINE const AudioBuffer* GetBuffer() const;

private:
    /// @brief The raw, uncompressed audio data.
    const uint8_t* m_Data = nullptr;
    /// @brief The size of the m_Data buffer.
    int32_t m_DataSize = 0;
    /// @brief The number of audio channels. This would be 1 for mono, 2 for stereo, and so on.
    uint16_t m_Channels = 0;
    /// @brief Also often called frequency.
    int32_t m_SampleRate = 0;
    /// @brief The number of bits per sample.
    uint16_t m_BitDepth = 0;

    AudioBuffer* m_Buffer = nullptr;
    
    XNOR_ENGINE bool_t LoadWavefront(const uint8_t* buffer, int64_t length);
    XNOR_ENGINE int64_t LoadWavefrontFormat(const uint8_t* data);
    XNOR_ENGINE int64_t LoadWavefrontData(const uint8_t* data);
};

END_XNOR_CORE

#include "resource/audio_track.inl"

REFL_AUTO(type(XnorCore::AudioTrack, bases<XnorCore::Resource>),
    field(m_DataSize),
    field(m_Channels),
    field(m_SampleRate),
    field(m_BitDepth)
)
