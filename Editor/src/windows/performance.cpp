#include "windows/performance.hpp"

#include <format>

#include "utils/windows.hpp"

#include <psapi.h>

#include "screen.hpp"
#include "GLFW/glfw3.h"
#include "imgui/imgui.h"
#include "input/time.hpp"
#include "Maths/calc.hpp"

using namespace XnorEditor;

Performance::Performance(Editor* const editor, const size_t sampleCount)
    : UiWindow(editor, "Performance Graphs")
{
    SetSampleCount(sampleCount);
}

void Performance::Update()
{
    m_LastFps = 1.f / XnorCore::Time::GetDeltaTime();
    m_FrameRateArray[m_ArrayIndex] = m_LastFps;
    m_HighestArrayFps = std::max(*std::ranges::max_element(m_FrameRateArray), static_cast<float_t>(XnorCore::Screen::GetRefreshRate()));

    PROCESS_MEMORY_COUNTERS memory;
    GetProcessMemoryInfo(GetCurrentProcess(), &memory, sizeof(memory));
    XnorCore::Windows::SilenceError();

    m_LastMemory = static_cast<float_t>(memory.WorkingSetSize) / 0x100000; // Convert to MB
    m_MemoryArray[m_ArrayIndex] = m_LastMemory;

    const decltype(m_MemoryArray)::const_iterator last = m_TotalSamples == m_MaxTotalSamples ? m_MemoryArray.end() : m_MemoryArray.cbegin() + static_cast<long long>(m_TotalSamples);
    m_HighestArrayMemory = *std::ranges::max_element(m_MemoryArray.cbegin(), last) * MemoryArrayBoundsFactor;
    m_LowestArrayMemory = *std::ranges::min_element(m_MemoryArray.cbegin(), last) / MemoryArrayBoundsFactor;

    m_ArrayIndex = static_cast<int32_t>((m_ArrayIndex + 1) % m_FrameRateArray.size());
    if (m_TotalSamples < m_MaxTotalSamples)
        m_TotalSamples++;
}

void Performance::Display()
{
    if (Calc::OnInterval(XnorCore::Time::GetTotalTime(), XnorCore::Time::GetLastTotalTime(), m_UpdateInterval))
        Update();
    
    const ImVec2 available = ImGui::GetContentRegionAvail();
    std::string format = std::format("FPS: {:.0f}", m_LastFps);

    ImGui::PlotLines("##fps", m_FrameRateArray.data(), static_cast<int32_t>(std::min(m_TotalSamples, m_FrameRateArray.size())), m_ArrayIndex,
        format.c_str(), 0.f, m_HighestArrayFps, ImVec2(available.x, GraphsHeight));

    format = std::format("Memory: {:.2f}MB", m_LastMemory);
    ImGui::PlotLines("##memory", m_MemoryArray.data(), static_cast<int32_t>(std::min(m_TotalSamples, m_MemoryArray.size())), m_ArrayIndex,
        format.c_str(), m_LowestArrayMemory, m_HighestArrayMemory, ImVec2(available.x, GraphsHeight));
}

void Performance::SetSampleCount(const size_t sampleCount)
{
    m_FrameRateArray.resize(sampleCount);
    m_MemoryArray.resize(sampleCount);

    m_MaxTotalSamples = sampleCount;
}
