#pragma once

#include <array>

#include "core.hpp"
#include "utils/meta_programming.hpp"

BEGIN_XNOR_CORE

template <typename T, size_t Size>
class LowPassFilter
{
public:
    using Type = T;

    DEFAULT_COPY_MOVE_OPERATIONS(LowPassFilter)
    
    LowPassFilter() = default;

    ~LowPassFilter() = default;

    template <typename U>
    U GetAvarage() const;
    
    void AddSample(T sample);

    void Reset();

private:
    std::array<Type, Size> m_Data;
    T m_Sum;
    size_t m_CurrentFrameIndex = 0;
};

END_XNOR_CORE

#include "input/low_pass_filter.inl"
