#pragma once

#include "core.hpp"
#include "scene/component.hpp"
#include "utils/list.hpp"

BEGIN_XNOR_CORE

namespace DotnetUtils
{
#ifndef SWIG
    XNOR_ENGINE const Component& ComponentListGetItem(const List<Component*>& list, size_t index, std::string* managedTypeName);
#endif
}

END_XNOR_CORE
