#pragma once

#include "core.hpp"

BEGIN_XNOR_CORE

template <typename T, size_t Size>
template <typename U>
U LowPassFilter<T, Size>::GetAvarage() const
{
    static_assert(Size != 0, "Size can't be equal to zero");
    
    return m_Sum / Size;
}

template <typename T, size_t Size>
void LowPassFilter<T, Size>::AddSample(T sample)
{
    m_Sum -= m_Data[m_CurrentFrameIndex];
    m_Data.at(m_CurrentFrameIndex) = sample;
    m_Sum += sample;
    m_CurrentFrameIndex += 1;

    if (m_CurrentFrameIndex >= Size)
        m_CurrentFrameIndex = 0;
}

template <typename T, size_t Size>
void LowPassFilter<T, Size>::Reset()
{
    for (T& t : m_Data)
    {
        t = {}; 
    }

    m_CurrentFrameIndex = 0;
    m_Sum = {};
}

END_XNOR_CORE
