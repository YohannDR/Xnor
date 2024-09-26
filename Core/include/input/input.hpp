#pragma once

#include <array>

#include "core.hpp"
#include "low_pass_filter.hpp"
#include "window.hpp"
#include "input/gamepad_input.hpp"
#include "input/keyboard_input.hpp"
#include "input/mouse_input.hpp"

/// @file input.hpp
/// @brief Defines the XnorCore::Input static class.

BEGIN_XNOR_CORE

/// @brief Used to fetch inputs from the current @ref Window.
class Input
{
    STATIC_CLASS(Input)
    
public:
    struct BoundWindowInfo
    {
        Vector2 windowPos;
        Vector2 windowSize;
        
        bool_t isEditor = false;
        bool_t isRendering = false;
    };
    
    /// @brief Whether the mouse is locked
    XNOR_ENGINE static inline bool_t mouseLocked = false;

    // @brief Whether the input are locked
    XNOR_ENGINE static void LockInput(bool_t lockInput);
    
    /// @brief Checks if a specified key is of the specified status
    /// @param key Key to check
    /// @param status Key status
    /// @return Result
    XNOR_ENGINE static bool_t GetKey(Key key, KeyStatus status = KeyStatus::Down);

    /// @brief Checks if a specified mouse button is of the specified status
    /// @param mouseButton Mouse button to check
    /// @param status Mouse button status
    /// @return Result
    XNOR_ENGINE static bool_t GetMouseButton(MouseButton mouseButton, MouseButtonStatus status = MouseButtonStatus::Down);

    /// @brief Checks if a specified gamepad button is of the specified status
    /// @param gamePadId Gamepad id
    /// @param gamepadButton Gamepad button to check
    /// @param buttonStatus Gamepad button status
    /// @return Result
    XNOR_ENGINE static bool_t GetGamepadButton(uint32_t gamePadId, GamepadButton gamepadButton, GamepadButtonStatus buttonStatus = GamepadButtonStatus::Down);

    /// @brief Processes the input events
    XNOR_ENGINE static void HandleEvents();

    /// @brief Gets the mouse position
    /// @tparam T Vector2 type
    /// @return Mouse position
    template <class T>
    static T GetMousePosition();

    XNOR_ENGINE static Vector2 GetMouseDelta();

    /// @brief Initializes the input manager
    XNOR_ENGINE static void Initialize();

    /// @brief Updates the input manager
    XNOR_ENGINE static void Update();

    XNOR_ENGINE static void Reset();

    // Return A array indexOf WindowInfo
    XNOR_ENGINE static uint32_t GetBindingId();

    XNOR_ENGINE static void UpdateBoundWindowInfo(uint32_t binding, const BoundWindowInfo& windowInfo);
    
    XNOR_ENGINE static void GetWindowBoundInfo(std::vector<BoundWindowInfo>* boundWindowInfos);

    XNOR_ENGINE static void BindWindow(uint32_t bindInd);

private:
    using KeyStatuses = std::array<bool_t, KeyStatusCount>;
    
    using MouseStatuses = std::array<bool_t, MouseButtonStatusCount>;
    
    XNOR_ENGINE static inline std::vector<BoundWindowInfo> m_BoundWindowInfo;
    
    XNOR_ENGINE static inline uint32_t m_CurrentBoundWindow = 0;

    static inline bool_t m_IsInputLocked = false;

    static constexpr size_t GamepadMax = 15;
    
    XNOR_ENGINE static inline std::array<KeyStatuses, KeyCount - 1> m_Keyboard;

    XNOR_ENGINE static inline std::array<MouseStatuses, MouseButtonCount - 1> m_Mouse;
    
    XNOR_ENGINE static inline std::array<GamepadInput, GamepadMax> m_Gamepads;

    // For each Status
    using GamepadButtonStatuses = std::array<bool_t, GamepadButtonStatusCount>;
    // For each Button
    using GamepadButtons = std::array<GamepadButtonStatuses, GamepadButtonCount>;
    // For each Gamepad
    XNOR_ENGINE static inline std::array<GamepadButtons, GamepadMax> m_GamepadsButton;
    
    XNOR_ENGINE static inline Vector2 m_LastMousePosition;
    
    XNOR_ENGINE static inline Vector2 m_MousePosition;
    
    XNOR_ENGINE static inline LowPassFilter<Vector2,6> m_MouseDelta;

    XNOR_ENGINE static inline GLFWwindow* m_WindowHandle = nullptr;

    static void HandleKeyboard(GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods);
    
    static void HandleMouseButton(GLFWwindow* window, int32_t mouseButton, int32_t action, int32_t mods);
    
    static void HandleJoyStickCallBack(int32_t jid, int32_t event);
    
    static void HandleGamepad();

    static void ResetKey();

    static void CheckGamepadAtLaunch();
};

END_XNOR_CORE

#include "input/input.inl"
