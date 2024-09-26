#pragma once

#include "core.hpp"
#include "rendering/renderer.hpp"
#include "utils/event.hpp"

/// @file application.hpp
/// @brief Defines the XnorCore::Application class.

BEGIN_XNOR_CORE

/// @brief Application interface, handles the lifetime of most of the sub systems needed for an XNOR application to run
class Application
{
public:
    /// @brief The path of the current executable
    ///
    /// This is valid once @ref Application::Application "the Application constructor" has been called
    XNOR_ENGINE static inline std::filesystem::path executablePath;

    /// @brief Current Application instance.
    XNOR_ENGINE static inline Application* applicationInstance = nullptr;
    
    /// @brief Exits the currently-executing Application immediately, destroying the current Application beforehand.
    [[noreturn]]
    XNOR_ENGINE static void Exit(int32_t code = 0);
    
    /// @brief Renderer of the application
    Renderer renderer;
    
    Viewport* gameViewPort = nullptr;

    /// @brief ctor, Initializes the sub systems
    XNOR_ENGINE Application(int32_t argc, const char_t* const* argv);

    /// @brief dtor, Destroys the sub systems
    XNOR_ENGINE virtual ~Application();

    DEFAULT_COPY_MOVE_OPERATIONS(Application)

    /// @brief Main loop of an application
    XNOR_ENGINE virtual void Update() = 0;

    /// @brief Should handle the window resize actions
    XNOR_ENGINE virtual void CheckWindowResize() = 0;
    
protected:
  
};

END_XNOR_CORE
