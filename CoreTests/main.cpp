#include "pch.hpp"

#include "window.hpp"
#include "utils/logger.hpp"

int32_t main(int32_t argc, char_t** const argv)
{
    testing::InitGoogleTest(&argc, argv);
    
    Logger::Start();
    
    // We might need GLFW for the Time::Update function which itself is needed for the Coroutine::UpdateAll function
    Window::Initialize();
    
    const int32_t result = RUN_ALL_TESTS();

    Window::Shutdown();
    
    Logger::Stop();

    return result;
}
