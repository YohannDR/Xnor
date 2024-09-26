#pragma once

#include "utils/meta_programming.hpp"

BEGIN_XNOR_CORE

template float_t Time::GetTotalTime<float_t>();
template double_t Time::GetTotalTime<double_t>();

template <typename T>
T Time::GetTotalTime()
{
    static_assert(Meta::IsFloatingPoint<T>, "You can only get total time as a floating point type");
    
    return static_cast<T>(m_TotalTime);
}

template float_t Time::GetLastTotalTime<float_t>();
template double_t Time::GetLastTotalTime<double_t>();

template <typename T>
T Time::GetLastTotalTime()
{
    static_assert(Meta::IsFloatingPoint<T>, "You can only get last total time as a floating point type");
    
    return static_cast<T>(m_LastTotalTime);
}

template float_t Time::GetDeltaTime<float_t>();
template double_t Time::GetDeltaTime<double_t>();

template <typename T>
T Time::GetDeltaTime()
{
    static_assert(Meta::IsFloatingPoint<T>, "You can only get delta time as a floating point type");
    
    return static_cast<T>(m_DeltaTime);
}

template uint32_t Time::GetTotalFrameCount<uint32_t>();
template uint64_t Time::GetTotalFrameCount<uint64_t>();
template int32_t Time::GetTotalFrameCount<int32_t>();

template <typename T>
T Time::GetTotalFrameCount()
{
    static_assert(Meta::IsIntegral<T>, "You can only get total frame count as an integral type");
    
    return static_cast<T>(m_TotalFrameCount);
}

END_XNOR_CORE
