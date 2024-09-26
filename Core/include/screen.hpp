#pragma once

#include <Maths/vector2i.hpp>

#include "core.hpp"
#include "window.hpp"

/// @file screen.hpp
/// @brief Defines the XnorCore::Screen class.

// ReSharper disable once CppInconsistentNaming
struct GLFWmonitor;
// ReSharper disable once CppInconsistentNaming
struct GLFWvidmode;

BEGIN_XNOR_CORE

/// @brief Represents a screen/monitor
class XNOR_ENGINE Screen
{
    STATIC_CLASS(Screen)
    
public:
    /// @brief Gets the size of the screen
    /// @return Screen size
    static Vector2i GetSize();

    /// @brief Gets the refresh rate of the screen
    /// @return Refresh rate
    static int32_t GetRefreshRate();

    /// @brief Initializes the screen data
    static void Initialize();

private:
    /// @brief Native handle for the monitor
    static inline GLFWmonitor* m_Monitor = nullptr;
    
    /// @brief Native handle for the monitor
    static inline const GLFWvidmode* m_VideoMode = nullptr;
};

END_XNOR_CORE
