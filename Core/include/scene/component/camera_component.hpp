#pragma once

#include "core.hpp"
#include "rendering/camera.hpp"
#include "scene/component.hpp"

/// @file camera_component.hpp
/// @brief Defines the XnorCore::CameraComponent class.

BEGIN_XNOR_CORE

/// @brief Component that encapsulates a @ref Camera
class XNOR_ENGINE CameraComponent : public Component
{
	REFLECTABLE_IMPL(CameraComponent)

public:
	/// @brief Camera
	Camera camera;

	CameraComponent();

	~CameraComponent() override;

	void Update() override;

	void Begin() override;
	
	void OnRendering() override;

private:
	void FollowEntityRotation();
};

END_XNOR_CORE

REFL_AUTO(type(XnorCore::CameraComponent, bases<XnorCore::Component>))
