#pragma once

#include <cmath>

#include "core.hpp"

/// @file time.hpp
/// @brief Defines the XnorCore::Time static class.

BEGIN_XNOR_CORE

/// @brief Defines utility functions to access information regarding time
class Time
{
    STATIC_CLASS(Time)

public:
    /// @brief Gets the total elapsed time
    /// @tparam T Result type
    /// @return Total time
    template <typename T = float_t>
    static T GetTotalTime();

    /// @brief Gets the last total elapsed time
    /// @tparam T Result type
    /// @return Last total time
    template <typename T = float_t>
    static T GetLastTotalTime();
    
    /// @brief Gets the delta time
    /// @tparam T Result type
    /// @return Delta time
    template <typename T = float_t>
    static T GetDeltaTime();

    /// @brief Gets the total frame count
    /// @tparam T Result type
    /// @return Total frame count
    template <typename T = uint32_t>
    static T GetTotalFrameCount();
    
    /// @brief Updates the time variables using GLFW.
    ///        This function should be called exactly once each frame.
    XNOR_ENGINE static void Update();

private:
    XNOR_ENGINE static inline double_t m_TotalTime = 0.0;
    XNOR_ENGINE static inline double_t m_LastTotalTime = 0.0;
    XNOR_ENGINE static inline double_t m_DeltaTime = 0.0;
    XNOR_ENGINE static inline uint64_t m_TotalFrameCount = 0;
};

END_XNOR_CORE

#include "input/time.inl"
