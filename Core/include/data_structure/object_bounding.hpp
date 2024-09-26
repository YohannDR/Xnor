#pragma once

#include "core.hpp"
#include "utils/bound.hpp"

BEGIN_XNOR_CORE

template<class T>
struct ObjectBounding
{
    T* handle = nullptr;
    
    Bound bound;

    DEFAULT_COPY_MOVE_OPERATIONS(ObjectBounding)

    bool operator==(const ObjectBounding& other) const
    {
        return bound == other.bound;
    }

    ObjectBounding() = default;

    ~ObjectBounding() = default;
};

END_XNOR_CORE