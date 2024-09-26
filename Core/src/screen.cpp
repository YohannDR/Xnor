#include "screen.hpp"

#include <GLFW/glfw3.h>

using namespace XnorCore;

Vector2i Screen::GetSize()
{
    Vector2i result;
    glfwGetMonitorWorkarea(m_Monitor, nullptr, nullptr, &result.x, &result.y); // Not sure if this returns a correct value (this is in screen coords not in pixels)
    return result;
}

int32_t Screen::GetRefreshRate()
{
    return m_VideoMode->refreshRate;
}

void Screen::Initialize()
{
    m_Monitor = glfwGetPrimaryMonitor();
    m_VideoMode = glfwGetVideoMode(m_Monitor);
}
