#pragma once

#include "frame_buffer.hpp"
#include "viewport_data.hpp"
#include "rendering/camera.hpp"
#include "render_systems/post_process_pass.hpp"
#include "render_systems/tone_mapping.hpp"
#include "scene/entity.hpp"

/// @file viewport.hpp
/// @brief Defines the XnorCore::Viewport class.

BEGIN_XNOR_CORE

/// @brief Class used to store everything needed to render a frame.
///
/// Stores the main @ref XnorCore::Viewport::camera "camera" along with the @ref XnorCore::Viewport::frameBuffer "frameBuffer".
class Viewport
{
public:
	/// @brief The Camera used to render a frame.
	const Camera* camera = nullptr;

	/// @brief Texture that's rendered to
	Texture* image = nullptr;

	/// @brief Viewport size
	Vector2i viewPortSize = Window::GetSize();

	/// @brief Color pass
	RenderPass colorPass;

	/// @brief Framebuffer
	Framebuffer* frameBuffer = nullptr;
	
	/// @brief Whether this is used in the editor.
	bool_t isEditor = false;

	/// @brief Additional viewport data
	ViewportData viewportData;

	/// @brief Initializes a viewport
	/// @param size Size
	XNOR_ENGINE void Init(Vector2i size);

	/// @brief Destroys the viewport
	XNOR_ENGINE void Destroy();

	/// @brief Resizes the viewport
	/// @param newSize New size
	XNOR_ENGINE void Resize(Vector2i newSize);
	

	[[nodiscard]]
	XNOR_ENGINE float_t GetAspect() const;
};

END_XNOR_CORE
