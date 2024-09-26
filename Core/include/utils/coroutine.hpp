#pragma once

#include <coroutine>
#include <functional>
#include <unordered_map>

#include "core.hpp"
#include "utils/guid.hpp"

/// @file coroutine.hpp
/// @brief Defines the Coroutine class.

BEGIN_XNOR_CORE

// Everything named using the snake_case convention in this file is required by C++20 coroutines: https://en.cppreference.com/w/cpp/language/coroutines

/// @brief Wrapper around C++20 Coroutines.
/// @see <a href="https://en.cppreference.com/w/cpp/language/coroutines">C++20 Coroutines</a>
class Coroutine
{
    struct Awaitable;
    
public:
    /// @brief Default duration type for Coroutine wait, equivalent to floating-point seconds
    using AwaitType = std::chrono::duration<double_t>;

    /// @brief Promise type for C++20 coroutines.
    ///
    /// This is necessary for the coroutine to function and needs to be public but mustn't be used
    /// manually.
    struct promise_type
    {
        /// @brief The last @c co_await value
        AwaitType awaitValue = AwaitType::zero();

        /// @brief Whether the Coroutine reached either a @c co_return statement, or the end of the function body.
        bool_t finished = false;

        /// @brief Returns the object that will be returns to the caller of a CoroutineFunc
        XNOR_ENGINE Coroutine get_return_object();

        /// @brief Empty implementation
        XNOR_ENGINE std::suspend_always initial_suspend();

        /// @brief Empty implementation
        XNOR_ENGINE std::suspend_always final_suspend() noexcept;

        /// @brief Logs the exception and rethrows it
        XNOR_ENGINE void unhandled_exception();

        /// @brief Called when @c co_return is used in a Coroutine body. Sets @c finished to @c true.
        XNOR_ENGINE void return_void();

        /// @brief Converts a @c AwaitType value to an @c Awaitable. Called when @c co_await is used with an @c AwaitType value.
        XNOR_ENGINE Awaitable await_transform(const AwaitType& duration);

        /// @brief Converts a @c float_t value to an @c Awaitable. Called when @c co_await is used with a @c float_t value.
        XNOR_ENGINE Awaitable await_transform(float_t duration);
    };
    
private:
    struct Awaitable
    {
        /// @brief Returns @c false if @c await_suspend should be called
        XNOR_ENGINE bool_t await_ready();

        /// @brief Empty implementation
        XNOR_ENGINE void await_suspend(std::coroutine_handle<promise_type>);

        /// @brief Empty implementation
        XNOR_ENGINE void await_resume();
    };

    XNOR_ENGINE static inline std::unordered_map<Guid, Coroutine> m_RunningRoutines;
    
public:
    /// @brief The coroutine handle type.
    using HandleType = std::coroutine_handle<promise_type>;

    XNOR_ENGINE static Guid Start(const Coroutine& coroutine);

    /// @brief Starts a coroutine using an existing coroutine Guid.
    ///
    /// This stops the existing coroutine if it is still running and assigns the guid with a newly created one.
    XNOR_ENGINE static void Start(const Coroutine& coroutine, Guid* coroutineId);

    XNOR_ENGINE static void UpdateAll();

    XNOR_ENGINE static void Stop(const Guid& coroutineId);

    XNOR_ENGINE static void StopAll();

    XNOR_ENGINE static bool_t IsRunning(const Guid& coroutineId);

    XNOR_ENGINE static bool_t IsRunningAndNotEmpty(const Guid& coroutineId);

    XNOR_ENGINE static size_t GetRunningCount();

    // ReSharper disable once CppNonExplicitConvertingConstructor
    /// @brief Constructs a new Coroutine from the given handle.
    ///
    /// This can be obtained as easily as this:
    /// @code
    /// Coroutine Function()
    /// {
    ///     using std::chrono_literals;
    ///     
    ///     // Do something
    ///     
    ///     co_await 750ms;
    ///
    ///     // Do something else
    /// }
    ///
    /// // Somewhere else
    /// Coroutine routine = Function();
    ///
    /// // Here you can do whatever you want with the Coroutine
    /// routine.Resume();
    /// @endcode
    XNOR_ENGINE Coroutine(HandleType handle);

    /// @brief Resumes the Coroutine.
    XNOR_ENGINE void Resume() const;

    /// @brief Returns whether the Coroutine finished its execution.
    /// @see promise_type::finished
    XNOR_ENGINE bool_t Finished() const;

    /// @brief Destroys the Coroutine. It can't be resumed afterwards.
    XNOR_ENGINE void Destroy() const;

    /// @brief Returns the Guid of this Coroutine.
    XNOR_ENGINE Guid GetId() const;

private:
    HandleType m_Handle;

    Guid m_Id;
    
    XNOR_ENGINE Coroutine(const Guid& guid, HandleType handle);
};

/// @brief Coroutine function prototype.
template <typename... Args>
using CoroutineFunc = std::function<Coroutine(Args...)>;

END_XNOR_CORE
