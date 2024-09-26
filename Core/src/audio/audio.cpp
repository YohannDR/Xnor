#include "audio/audio.hpp"

#include <set>

#include <AL/alc.h>

#include "utils/logger.hpp"

using namespace XnorCore;

bool_t Audio::Initialize()
{
    Logger::LogInfo("Initializing audio");
    
    InitializeDevices();
    
    m_CurrentContext = new AudioContext(*m_CurrentDevice);

    return true;
}

void Audio::Shutdown()
{
    Logger::LogInfo("Shutting down audio");

    for (auto&& buffer : m_Buffers)
        delete buffer;

    for (auto&& device : m_AvailableDevices)
        delete device;
}

AudioContext* Audio::GetContext() { return m_CurrentContext; }

void Audio::RegisterBuffer(AudioBuffer* buffer) { m_Buffers.Add(buffer); }

void Audio::UnregisterBuffer(AudioBuffer* buffer) { m_Buffers.Remove(buffer); }

void Audio::UpdateContext()
{
    delete m_CurrentContext;
    m_CurrentContext = new AudioContext(*m_CurrentDevice);
}

const List<AudioDevice*>& Audio::GetAvailableDevices() { return m_AvailableDevices; }

const AudioDevice* Audio::GetCurrentDevice() { return m_CurrentDevice; }

void Audio::SetCurrentDevice(AudioDevice* newCurrentDevice)
{
    m_CurrentDevice = newCurrentDevice;
    UpdateContext();
}

void Audio::InitializeDevices()
{
    // Use std::set to make sure we don't create duplicate devices
    std::set<std::string> deviceNameList;

    // Get all available devices if the extension is present (this should always be the case)
    if (alcIsExtensionPresent(nullptr, "ALC_ENUMERATE_ALL_EXT"))
    {
        const char_t* const deviceNames = alcGetString(nullptr, ALC_ALL_DEVICES_SPECIFIER);
        IterateAlStringList(deviceNames, [&](const char_t* const str) { deviceNameList.emplace(str); });
    }

    // Just in case the enumeration extension isn't present, add the default device
    deviceNameList.emplace(alcGetString(nullptr, ALC_DEFAULT_ALL_DEVICES_SPECIFIER));

    // Create the devices and add them to the list
    const size_t size = deviceNameList.size();
    m_AvailableDevices.Resize(size);
    auto&& it = deviceNameList.begin();
    for (auto& device : m_AvailableDevices)
    {
        device = new AudioDevice(std::string(*it));
        it++;
    }

    m_CurrentDevice = m_AvailableDevices.Front();
}

void Audio::IterateAlStringList(const char_t* list, const std::function<void(const char_t*)>& lambda)
{
    for (int32_t i = 0; list[i] != '\0' || list[i + 1] != '\0'; i++)
    {
        if (i == 0 || list[i - 1] == '\0')
            lambda(list + i);
    }
}
