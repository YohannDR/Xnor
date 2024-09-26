#pragma once

#include <vector>

#include "windows/ui_window.hpp"

BEGIN_XNOR_EDITOR

class Performance : public UiWindow
{
    // Constants
    static constexpr float_t GraphsHeight = 50.f;
    static constexpr uint32_t DefaultSampleCount = 50;
    static constexpr float_t MemoryArrayBoundsFactor = 1.1f;
    
public:
    explicit Performance(Editor* editor, size_t sampleCount);
    
    void Update();

    void Display() override;

    void SetSampleCount(size_t sampleCount);

private:
    float_t m_UpdateInterval = 0.25f;
    
    size_t m_TotalSamples = 0;
    size_t m_MaxTotalSamples = 0;
    int32_t m_ArrayIndex = 0;

    float_t m_LastFps = 0.f;
    std::vector<float_t> m_FrameRateArray;
    float_t m_HighestArrayFps = 0.f;
        
    float_t m_LastMemory = 0.f;
    std::vector<float_t> m_MemoryArray;
    float_t m_HighestArrayMemory = 0.f;
    float_t m_LowestArrayMemory = 0.f;
};

END_XNOR_EDITOR
