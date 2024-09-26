#pragma once

#include "core.hpp"
#include "Maths/vector2i.hpp"
#include "scene/component.hpp"

BEGIN_XNOR_CORE

class GuiComponent : public Component
{
    REFLECTABLE_IMPL(GuiComponent)

public:
    GuiComponent() = default;
    ~GuiComponent() override = default;

    DEFAULT_COPY_MOVE_OPERATIONS(GuiComponent)

    Vector2 screenTransform;

    Vector2 size = { 1.f, 1.f };
};

END_XNOR_CORE

REFL_AUTO(
    type(XnorCore::GuiComponent, bases<XnorCore::Component>),
    field(screenTransform),
    field(size)
);
