#include "input/input.hpp"

#include <GLFW/glfw3.h>

#include "utils/logger.hpp"
#include "utils/windows.hpp"

using namespace XnorCore;

void Input::LockInput(const bool_t lockInput)
{
    m_IsInputLocked = lockInput;
}

bool_t Input::GetKey(const Key key, const KeyStatus status)
{
    if (m_IsInputLocked)
        return false;
    
    return m_Keyboard.at(static_cast<size_t>(key)).at(static_cast<size_t>(status));
}

bool_t Input::GetMouseButton(const MouseButton mouseButton, const MouseButtonStatus status)
{
    if (m_IsInputLocked)
        return false;
    
    return m_Mouse.at(static_cast<size_t>(mouseButton)).at(static_cast<size_t>(status));
}

bool_t Input::GetGamepadButton(const uint32_t gamePadId, const GamepadButton gamepadButton, const GamepadButtonStatus buttonStatus)
{
    if (m_IsInputLocked)
        return false;
    
    if (gamePadId > GamepadMax)
    {
        Logger::LogFatal("Invalid gamepad ID {}", gamePadId);
        throw std::invalid_argument("Invalid gamepad ID");
    }

    return m_GamepadsButton.at(static_cast<size_t>(gamePadId)).at(static_cast<size_t>(gamepadButton)).at(static_cast<size_t>(buttonStatus));
}

void Input::HandleEvents()
{
    if (m_IsInputLocked)
        return;
    
    HandleGamepad();
}

void Input::HandleKeyboard(GLFWwindow*, const int32_t key, const int32_t, const int32_t action, const int32_t)
{
    if (static_cast<size_t>(key) > m_Keyboard.size())
        return;
    
    
    KeyStatuses& keyStatuses = m_Keyboard.at(static_cast<size_t>(key));
    
    switch (action)
    {
        case GLFW_RELEASE:
            keyStatuses.at(static_cast<size_t>(KeyStatus::Down)) = false;
            keyStatuses.at(static_cast<size_t>(KeyStatus::Release)) = true;
            keyStatuses.at(static_cast<size_t>(KeyStatus::Repeat)) = false;
            break;
        
        case GLFW_PRESS:
            keyStatuses.at(static_cast<size_t>(KeyStatus::Pressed)) = true;
            keyStatuses.at(static_cast<size_t>(KeyStatus::Down)) = true;
            break;
        
        case GLFW_REPEAT:
            keyStatuses.at(static_cast<size_t>(KeyStatus::Repeat)) = true;
            break;
        
        default:
            break;
    }
}

void Input::HandleMouseButton(GLFWwindow*, const int32_t mouseButton, const int32_t action, int32_t)
{
    MouseStatuses& keyStatuses = m_Mouse.at(static_cast<size_t>(mouseButton));
    
    switch (action)
    {
        case GLFW_RELEASE:
            keyStatuses.at(static_cast<size_t>(MouseButtonStatus::Down)) = false;
            keyStatuses.at(static_cast<size_t>(MouseButtonStatus::Release)) = true;
            break;

        case GLFW_PRESS:
            keyStatuses.at(static_cast<size_t>(MouseButtonStatus::Pressed)) = true;
            keyStatuses.at(static_cast<size_t>(MouseButtonStatus::Down)) = true;
            break;

        default:
            break;
    }
}

void Input::HandleJoyStickCallBack(const int32_t jid, const int32_t event)
{
    switch (event)
    {
        case GLFW_CONNECTED:
            m_Gamepads[jid].isConnected = true;
            break;

        case GLFW_DISCONNECTED:
            m_Gamepads[jid].isConnected = false;
            break;
        
        default:
            break;
    }
}
void Input::HandleGamepad()
{
    for (uint32_t i = 0; i < GamepadMax; i++)
    {
        if (!m_Gamepads[i].isConnected)
            continue;

        GLFWgamepadstate state;

        if (!glfwGetGamepadState(static_cast<int32_t>(i), &state))
            return;
        
        for (uint32_t k = 0; k < static_cast<uint32_t>(GamepadAxis::Count); k++)
        {
            if (state.axes[k] <= GamepadInput::NullAnalogValue)
                continue;

            m_Gamepads[i].axisValues[k] = state.axes[k];
        }

        for (uint32_t k = 0; k < static_cast<uint32_t>(GamepadButton::Count) - 1; k++)
        {
            GamepadButtonStatuses& statuses = m_GamepadsButton[i].at(static_cast<size_t>(k));
            const bool_t isDown = statuses.at(static_cast<size_t>(GamepadButtonStatus::Down));
            const bool_t isPress = statuses.at(static_cast<size_t>(GamepadButtonStatus::Pressed));
            
            switch (state.buttons[k])
            {
                case GLFW_RELEASE:
                    if (isDown || isPress)
                    {
                        statuses.at(static_cast<size_t>(GamepadButtonStatus::Release)) = true;
                        statuses.at(static_cast<size_t>(GamepadButtonStatus::Down)) = false;
                        statuses.at(static_cast<size_t>(GamepadButtonStatus::Pressed)) = false;
                    }
                    break;
                
                case GLFW_PRESS:
                    statuses.at(static_cast<size_t>(GamepadButtonStatus::Release)) = false;
                    statuses.at(static_cast<size_t>(GamepadButtonStatus::Down)) = true;
                    statuses.at(static_cast<size_t>(GamepadButtonStatus::Pressed)) = true;
                    break;
                
                default:
                    break;
            }
        }
    }
}

void Input::ResetKey()
{
    glfwSetKeyCallback(m_WindowHandle, HandleKeyboard);
    glfwSetMouseButtonCallback(m_WindowHandle, HandleMouseButton);

    KeyStatuses defaultKeys;
    defaultKeys.fill(false);
    m_Keyboard.fill(defaultKeys);

    MouseStatuses defaultMouseButtons;
    defaultMouseButtons.fill(false);
    m_Mouse.fill(defaultMouseButtons);
}

void Input::CheckGamepadAtLaunch()
{
    for (int32_t i = 0; i < static_cast<int32_t>(GamepadMax); i++)
    {
        const int32_t present = glfwJoystickPresent(i);
        m_Gamepads.at(static_cast<size_t>(i)).isConnected = static_cast<bool_t>(present);
    }
}

void Input::Update()
{
    if (m_IsInputLocked)
    {
        m_MouseDelta = {};
        m_LastMousePosition = {};
        m_MousePosition = {};
        return;
    }
    
    POINT point;
    GetCursorPos(&point);
    Windows::SilenceError();
    float_t mouseX = static_cast<float_t>(point.x);
    float_t mouseY = static_cast<float_t>(point.y);
    m_MousePosition = { mouseX, mouseY };

    m_MouseDelta.AddSample(m_MousePosition - m_LastMousePosition);

    if (mouseLocked)
        m_MouseDelta.AddSample(-m_MouseDelta.GetAvarage<Vector2>());

    m_LastMousePosition = m_MousePosition; 
}

void Input::Reset()
{
    for (auto& button : m_Mouse)
    {
        button.at(static_cast<size_t>(MouseButtonStatus::Pressed)) = false;
        button.at(static_cast<size_t>(MouseButtonStatus::Release)) = false;
    }

    for (auto& key : m_Keyboard)
    {
        key.at(static_cast<size_t>(KeyStatus::Pressed)) = false;
        key.at(static_cast<size_t>(KeyStatus::Release)) = false;
        key.at(static_cast<size_t>(KeyStatus::Repeat)) = false;
    }

    for (auto& gamepad : m_GamepadsButton)
    {
        for (auto& button : gamepad)
        {
            button.at(static_cast<size_t>(GamepadButtonStatus::Pressed)) = false;
            button.at(static_cast<size_t>(GamepadButtonStatus::Release)) = false;
        }
    }
}

uint32_t Input::GetBindingId()
{
    const size_t index = m_BoundWindowInfo.size();
    m_BoundWindowInfo.resize(index + 1);
    return static_cast<uint32_t>(index);
}

void Input::UpdateBoundWindowInfo(const uint32_t binding, const BoundWindowInfo& windowInfo)
{
    m_BoundWindowInfo[static_cast<size_t>(binding)] = windowInfo;
}

void Input::GetWindowBoundInfo(std::vector<BoundWindowInfo>* boundWindowInfos)
{
    *boundWindowInfos = m_BoundWindowInfo;
}

void Input::BindWindow(const uint32_t bindInd)
{
    if (m_BoundWindowInfo.size() > bindInd)
        return;

    m_CurrentBoundWindow = bindInd;
}

Vector2 Input::GetMouseDelta()
{
    return m_MouseDelta.GetAvarage<Vector2>();
}

void Input::Initialize()
{
    GLFWwindow* const windowPtr = glfwGetCurrentContext();

    glfwSetKeyCallback(windowPtr, HandleKeyboard);
    glfwSetMouseButtonCallback(windowPtr, HandleMouseButton);
    glfwSetJoystickCallback(HandleJoyStickCallBack);

    KeyStatuses defaultKeys;
    defaultKeys.fill(false);
    m_Keyboard.fill(defaultKeys);

    MouseStatuses defaultMouseButtons;
    defaultMouseButtons.fill(false);
    m_Mouse.fill(defaultMouseButtons);

    CheckGamepadAtLaunch();
}
