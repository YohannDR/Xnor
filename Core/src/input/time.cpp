#include "input/time.hpp"

#include <GLFW/glfw3.h>

using namespace XnorCore;

void Time::Update()
{
    m_LastTotalTime = m_TotalTime;
    
    m_TotalTime = glfwGetTime();
    
    m_DeltaTime = m_TotalTime - m_LastTotalTime;

    m_TotalFrameCount++;
}
