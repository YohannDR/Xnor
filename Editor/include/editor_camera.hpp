#pragma once

#include "core.hpp"
#include "definitions.hpp"
#include "input/low_pass_filter.hpp"
#include "rendering/camera.hpp"

BEGIN_XNOR_EDITOR

class Editor;

class EditorCamera
{
    static constexpr float_t MaxPitch = 89.0f;

public:
    EditorCamera(Editor& editor, XnorCore::Camera& camera);
    
    void UpdateCamera();
    
    void OnPressGoToObject();
    
private:
    Editor* m_EditorRef = nullptr;
    XnorCore::Camera* m_EditorRefCamera = nullptr;
    float_t m_DistanceToStop = 0.f;
    
    Vector2 m_MouseDrag;
    Vector2 m_PreviousMouseDrag;
    Vector2 m_MouseDelta;
    
    float_t m_MouseSensitivity = 0.1f;
    float_t m_Yaw = -90.f;
    float_t m_Pitch = 0.f;
    float_t m_CameraSpeed = 4.5f;

    bool_t m_IsMoving = false;
    bool_t m_FirstMove = false;

    float_t m_MouseWheelZoom = 50.f;

    XnorCore::LowPassFilter<Vector2, 6> m_LowPassFilterDeltaMouse {};
    
    void CameraOnRightClick();
    
    void EditorCameraRotation();

    void EditorCameraMovement();

    void OnMiddleButton();

    void GoToObject();

    void AddMovement(const Vector3& movement);

    void ClampMouseToScreen(Vector2* currentMousePos , Vector2* previousMousePos);
};

END_XNOR_EDITOR
