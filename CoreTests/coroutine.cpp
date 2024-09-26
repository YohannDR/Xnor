#include "pch.hpp"

#include "window.hpp"
#include "input/time.hpp"
#include "utils/logger.hpp"

Coroutine WaitRoutine()
{
    using namespace std::chrono_literals;

    DEBUG_LOG("First Coroutine log");
    
    co_await 0.25s;

    DEBUG_LOG("Second Coroutine log");
    
    co_await 0s;

    DEBUG_LOG("Third Coroutine log");
    
    co_await 250ms;

    DEBUG_LOG("Fourth Coroutine log");

    co_return;
}

TEST(Coroutine, General)
{
    const Coroutine c = WaitRoutine();
    
    DEBUG_LOG("First outside log");
    
    c.Resume();
    
    DEBUG_LOG("Second outside log");
    
    c.Resume();
    
    DEBUG_LOG("Third outside log");
    
    c.Resume();
    
    DEBUG_LOG("Fourth outside log");
    
    c.Resume();
    
    DEBUG_LOG("Fifth outside log");
}

TEST(Coroutine, Manager)
{
    const Guid id = Coroutine::Start(WaitRoutine());
    
    DEBUG_LOG("First outside log");

    while (Coroutine::IsRunning(id))
    {
        Time::Update();
        Coroutine::UpdateAll();
    }

    DEBUG_LOG("Second outside log");
}
