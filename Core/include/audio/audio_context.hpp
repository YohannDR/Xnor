#pragma once

#include "core.hpp"
#include "utils/list.hpp"

// ReSharper disable once CppInconsistentNaming
struct ALCcontext;

BEGIN_XNOR_CORE

class AudioDevice;

enum class AudioSourceType
{
    Mono,
    Stereo
};

class AudioContext
{
public:
    [[nodiscard]]
    XNOR_ENGINE explicit AudioContext(AudioDevice& device);

    XNOR_ENGINE ~AudioContext();

    DELETE_COPY_MOVE_OPERATIONS(AudioContext)

    XNOR_ENGINE void MakeCurrent() const;

    XNOR_ENGINE static bool_t CheckError();

    [[nodiscard]]
    XNOR_ENGINE int32_t GetMaxSourceCount(AudioSourceType sourceType) const;

    /// @brief Returns the next available source of the given type.
    [[nodiscard]]
    XNOR_ENGINE uint32_t GetSource(AudioSourceType type = AudioSourceType::Mono);

private:
    ALCcontext* m_Handle = nullptr;
    AudioDevice* m_Device = nullptr;

    List<int32_t> m_Attributes;

    List<uint32_t> m_SourcesMono;
    List<uint32_t> m_SourcesStereo;
};

END_XNOR_CORE
