#pragma once

#include <map>

#include "core.hpp"
#include "event.hpp"

BEGIN_XNOR_CORE

template <typename... Args>
struct TimelineEvents
{
    Event<Args...> begin;
    Event<float_t, Args...> update;
    Event<Args...> ended;
    float_t duration = 0.f;
};

template <typename... Args>
class Timeline
{
    REFLECTABLE_IMPL(Timeline)
    
public:
    using BeginFunctionT = typename Event<Args...>::StdFunctionT;
    using UpdateFunctionT = typename Event<float_t, Args...>::StdFunctionT;
    using EndFunctionT = typename Event<Args...>::StdFunctionT;

    Timeline() = default;
    explicit Timeline(float_t duration);
    ~Timeline() = default;

    DEFAULT_COPY_MOVE_OPERATIONS(Timeline)
    
    void Start();
    bool_t Update(Args&&... args);

    void SetEventDuration(float_t when, float_t duration);
    void AddBeginEvent(float_t when, BeginFunctionT function);
    void AddUpdateEvent(float_t when, UpdateFunctionT function);
    void AddEndEvent(float_t when, EndFunctionT function);

    [[nodiscard]]
    float_t GetDuration() const;
    void SetDuration(float_t duration);

private:
    std::map<float_t, TimelineEvents<Args...>> m_Events;
    float_t m_Duration = 0.f;
    float_t m_Time = 0.f;
    float_t m_CurrentEventDuration = 0.f;

    typename decltype(m_Events)::iterator m_CurrentEvent;
    typename decltype(m_Events)::iterator m_NextEvent;
    bool_t m_HasCurrentEvent = false;
};

END_XNOR_CORE

REFL_AUTO(template((typename... Args), (XnorCore::TimelineEvents<Args...>)),
    field(begin),
    field(update),
    field(ended),
    field(duration)
)

REFL_AUTO(template((typename... Args), (XnorCore::Timeline<Args...>)),
    field(m_Events),
    field(m_Duration),
    field(m_Time)
)

#include "utils/timeline.inl"
