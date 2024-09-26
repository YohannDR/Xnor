#include "window.hpp"

#include <GLFW/glfw3.h>

#include "utils/logger.hpp"
#include "utils/windows.hpp"

using namespace XnorCore;

void Window::Initialize()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

	m_Window = glfwCreateWindow(m_Size.x, m_Size.y, "XNOR Engine", nullptr, nullptr);

	glfwSetFramebufferSizeCallback(m_Window, GlfwResizeFramebuffer);

	glfwMakeContextCurrent(m_Window);

	glfwSwapInterval(1); // Enable vsync
}

void Window::Shutdown()
{
	glfwDestroyWindow(m_Window);

	glfwTerminate();
}

Vector2i Window::GetSize()
{
	return m_Size;
}

bool_t Window::ShouldClose()
{
	return glfwWindowShouldClose(m_Window);
}

void Window::PollEvents()
{
	glfwPollEvents();
}

void Window::MakeContextCurrent()
{
	glfwMakeContextCurrent(m_Window);
}

GLFWwindow* Window::GetHandle()
{
	return m_Window;
}

void Window::Show()
{
	glfwShowWindow(m_Window);
}

void Window::Hide()
{
	glfwHideWindow(m_Window);
}

void Window::SetIcon(Texture& icon)
{
	if (icon.GetChannels() != 4)
	{
		Logger::LogError("Invalid texture for window icon {}", icon.GetName());
		return;
	}
	
	const Vector2i size = icon.GetSize();

	const GLFWimage image
	{
		.width = size.x,
		.height = size.y,
		.pixels = icon.GetData<uint8_t>()
	};
	
	glfwSetWindowIcon(m_Window, 1, &image);
}

void Window::SetCursorHidden(const bool_t value)
{
	ShowCursor(!value);
    Windows::SilenceError();
}

void Window::SetCursorPosition(const Vector2 newPosition)
{
	SetCursorPos(static_cast<int32_t>(newPosition.x),static_cast<int32_t>(newPosition.y));
    Windows::SilenceError();
}

void Window::GlfwResizeFramebuffer(GLFWwindow*, const int32_t width, const int32_t height)
{
	m_Size.x = width;
	m_Size.y = height;

	while (m_Size.x == 0 || m_Size.y == 0)
	{
		glfwGetFramebufferSize(m_Window, &m_Size.x, &m_Size.y);

		glfwWaitEvents();
	}
	
	resizeFrameBuffer = true;
}
