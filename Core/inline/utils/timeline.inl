#pragma once

#include "input/time.hpp"
#include "utils/logger.hpp"

BEGIN_XNOR_CORE

template <typename... Args>
Timeline<Args...>::Timeline(const float_t duration)
    : m_Duration(duration)
{
}

template <typename... Args>
void Timeline<Args...>::Start()
{
    m_Time = 0.f;
    m_CurrentEvent = m_Events.begin();
    m_NextEvent = m_Events.begin();
    m_HasCurrentEvent = false;
}

template <typename... Args>
bool_t Timeline<Args...>::Update(Args&&... args)
{
    m_Time += Time::GetDeltaTime();
    m_CurrentEventDuration += Time::GetDeltaTime();

    if (m_Time >= m_Duration)
        return true;

    if (m_HasCurrentEvent)
    {
        const float_t eventDuration = m_CurrentEvent->second.duration;
        if (m_CurrentEventDuration >= eventDuration)
        {
            m_CurrentEvent->second.ended.Invoke(std::forward<Args>(args)...);
            m_HasCurrentEvent = false;
        }
        else
        {
            m_CurrentEvent->second.update.Invoke(m_CurrentEventDuration / eventDuration, std::forward<Args>(args)...);
        }
    }

    if (m_NextEvent == m_Events.end())
        return false;

    if (m_NextEvent->first <= m_Time)
    {
        m_CurrentEvent = m_NextEvent;
        m_HasCurrentEvent = true;

        m_CurrentEvent->second.begin.Invoke(std::forward<Args>(args)...);

        ++m_NextEvent;
    }

    return false;
}

template <typename... Args>
void Timeline<Args...>::SetEventDuration(const float_t when, const float_t duration)
{
    m_Events[when].duration = duration;
}

template <typename... Args>
void Timeline<Args...>::AddBeginEvent(const float_t when, const BeginFunctionT function)
{
    if (when >= m_Duration)
    {
        Logger::LogInfo("Can't add an event that happens after the end of the timeline : {} >= {}", when, m_Duration);
        return;
    }

    m_Events[when].begin += function;
}

template <typename... Args>
void Timeline<Args...>::AddUpdateEvent(const float_t when, const UpdateFunctionT function)
{
    if (when >= m_Duration)
    {
        Logger::LogInfo("Can't add an event that happens after the end of the timeline : {} >= {}", when, m_Duration);
        return;
    }

    m_Events[when].update += function;
}

template <typename... Args>
void Timeline<Args...>::AddEndEvent(const float_t when, const EndFunctionT function)
{
    if (when >= m_Duration)
    {
        Logger::LogInfo("Can't add an event that happens after the end of the timeline : {} >= {}", when, m_Duration);
        return;
    }

    m_Events[when].ended += function;
}

template <typename... Args>
float_t Timeline<Args...>::GetDuration() const
{
    return m_Duration;
}

template <typename... Args>
void Timeline<Args...>::SetDuration(const float_t duration)
{
    if (m_Duration > duration)
    {
        // ...
    }
    
    m_Duration = duration;
}

END_XNOR_CORE
