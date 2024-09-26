#pragma once

#include <Maths/vector2i.hpp>

#include "core.hpp"
#include "resource/texture.hpp"

/// @file window.hpp
/// @brief Defines the XnorCore::Window class.

// ReSharper disable once CppInconsistentNaming
struct GLFWwindow;

BEGIN_XNOR_CORE

/// @brief A wrapper for the main window
class XNOR_ENGINE Window
{
	STATIC_CLASS(Window)
	
public:
	/// @brief Whether the frame buffer was resized last frame
	static inline bool_t resizeFrameBuffer = false;

	/// @brief Initializes the window
	static void Initialize();

	/// @brief Terminates the window
	static void Shutdown();

	/// @brief Gets the size in pixels of the window
	/// @return Size
	[[nodiscard]]
	static Vector2i GetSize();

	/// @brief Gets whether the window should close
	/// @return Should close
	[[nodiscard]]
	static bool_t ShouldClose();

	/// @brief Polls the events of the window
	static void PollEvents();

	/// @brief Sets the window to be the current context
	static void MakeContextCurrent();
	
	/// @brief Gets the native handle of the window
	/// @return Native handle
	[[nodiscard]]
	static GLFWwindow* GetHandle();

	/// @brief Displays the window
	static void Show();

	/// @brief Hides the window
	static void Hide();

	/// @brief Sets the icon for the window
	/// @param icon Icon
	static void SetIcon(Texture& icon);

	/// @brief Handles hiding or displaying the cursor
	/// @param value @c true hides, @c false displays
	static void SetCursorHidden(bool_t value);

	/// @brief Sets the mouse cursor position in screen space.
	static void SetCursorPosition(Vector2 newPosition);

private:
	/// @brief Callback when the window is resized
	/// @param window Native window handle
	/// @param width New width
	/// @param height New height
	static void GlfwResizeFramebuffer(GLFWwindow* window, int32_t width, int32_t height);

	/// @brief Native window handle
	static inline GLFWwindow* m_Window = nullptr;

	/// @brief Window size
	static inline Vector2i m_Size = { 1280, 720 };
};
	
END_XNOR_CORE
