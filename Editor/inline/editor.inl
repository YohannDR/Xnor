#pragma once

#include "utils/meta_programming.hpp"

template <typename T, typename... Args>
T* Editor::OpenWindow(Args&&... args)
{
    static_assert(XnorCore::Meta::IsBaseOf<UiWindow, T>, "Provided type wasn't a UI window");

    T* window = new T(this, std::forward<Args>(args)...);
    m_UiWindows.push_back(window);
    return window;
}

template <typename T, typename... Args>
T* Editor::SetupWindow(Args&&... args)
{
    static_assert(XnorCore::Meta::IsBaseOf<UiWindow, T>, "Provided type wasn't a UI window");

    T* const w = new T(this, std::forward<Args>(args)...);
    m_UiWindows.push_back(w);
    w->opened = false;
    w->canClose = true;
    return w;
}
