#pragma once

#include "core.hpp"
#include "gui_component.hpp"
#include "image.hpp"
#include "utils/event.hpp"

BEGIN_XNOR_CORE

class Button : public Component
{
    REFLECTABLE_IMPL(Button)

public:
    Image* componentTarget { nullptr };

    ~Button() override = default;

    void Update() override;

    Event<> onClick;
    
private:
    void ButtonUpdate() const;
};

END_XNOR_CORE

REFL_AUTO(
    type(XnorCore::Button, bases<XnorCore::Component>),
    field(onClick, XnorCore::Reflection::NotSerializable())
);
