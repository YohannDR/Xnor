#pragma once

#include <string>

#include "core.hpp"
#include "audio/audio_context.hpp"

// ReSharper disable once CppInconsistentNaming
struct ALCdevice;

BEGIN_XNOR_CORE

class AudioDevice
{
public:
    XNOR_ENGINE static bool_t CheckError(const AudioDevice* device = nullptr);

    [[nodiscard]]
    XNOR_ENGINE explicit AudioDevice(std::string&& name);

    XNOR_ENGINE ~AudioDevice();

    DELETE_COPY_MOVE_OPERATIONS(AudioDevice)

    [[nodiscard]]
    XNOR_ENGINE std::string GetName() const;

private:
    ALCdevice* m_Handle = nullptr;
    std::string m_Name;

    // AudioContext needs to access the handle of this class
    friend class AudioContext;
};

END_XNOR_CORE
