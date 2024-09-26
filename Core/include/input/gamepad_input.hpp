#pragma once

#include <array>
#include <cmath>

#include "core.hpp"

/// @file gamepad_input.hpp
/// @brief Defines gamepad input types and enumerations.
/// 
/// Defines the XnorCore::GamepadInput struct along with the XnorCore::GamepadAxis and XnorCore::GamepadButton enumerations.

BEGIN_XNOR_CORE

/// @brief Gamepad axis enumeration
enum class GamepadAxis
{
    LeftStickHorizontalAxis,
    LeftStickVerticalAxis,
    RightStickHorizontalAxis,
    RightStickVerticalAxis,
    LeftTriggerAxis,
    RightTriggerAxis,

    Count
};

constexpr size_t GamepadAxisCount = static_cast<size_t>(GamepadAxis::Count);

/// @brief Gamepad button enumeration
///
/// Defines the gamepad button values according to the Xbox controller button placements.
enum class GamepadButton
{
    A,               
    B,               
    X,              
    Y,               
    LeftBumper,     
    RightBumper,    
    Back,            
    Start,           
    Guide,          
    LeftThumb,      
    RightThumb,     
    DpadUp,         
    DpadRight,     
    DpadDown,      
    DpadLeft, 
    Last       = DpadLeft,
    Cross      = A,
    Circle     = B,
    Square     = X,
    Triangle   = Y,

    Count = 20
};

constexpr size_t GamepadButtonCount = static_cast<size_t>(GamepadButton::Count);

/// @brief Gamepad button
enum class GamepadButtonStatus
{
    /// @brief Pressed
    Pressed,
    /// @brief Held down
    Down,
    /// @brief Released
    Release,
    
    Count
};

constexpr size_t GamepadButtonStatusCount = static_cast<size_t>(GamepadButtonStatus::Count);

/// @brief Information about a gamepad
struct GamepadInput
{
    /// @brief Threshold that dictates that a stick analog value becomes 0.
    static constexpr float_t NullAnalogValue = 1.5259022e-05f;

    /// @brief Whether the gamepad is connected.
    bool_t isConnected = false;
    /// @brief Array of axis analog values.
    std::array<float_t, static_cast<uint32_t>(GamepadAxis::Count)> axisValues{};
};

END_XNOR_CORE
