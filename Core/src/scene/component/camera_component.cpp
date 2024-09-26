#include "scene/component/camera_component.hpp"

#include "application.hpp"

using namespace  XnorCore;

CameraComponent::CameraComponent()
{
    Application::applicationInstance->gameViewPort->camera = &camera;
}

CameraComponent::~CameraComponent()
{
    if (Application::applicationInstance->gameViewPort->camera == &camera)
        Application::applicationInstance->gameViewPort->camera = nullptr;
}

void CameraComponent::Update()
{
    Component::Update();
}

void CameraComponent::Begin()
{
    Component::Begin();
}

void CameraComponent::OnRendering()
{
    Component::OnRendering();
    FollowEntityRotation();
  
}

void CameraComponent::FollowEntityRotation()
{
    const Vector3 pos = static_cast<Vector3>(entity->transform.worldMatrix[3]);
    camera.position = pos;
    const Vector3 forward = entity->transform.GetForward();
    camera.LookAt(pos - forward);
}
