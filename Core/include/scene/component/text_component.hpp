#pragma once

#include "gui_component.hpp"
#include "resource/font.hpp"
#include "scene/component.hpp"
#include "utils/color.hpp"

BEGIN_XNOR_CORE

class TextComponent : public GuiComponent
{
public:
    TextComponent() = default;
    ~TextComponent() override = default;

    DEFAULT_COPY_MOVE_OPERATIONS(TextComponent)
    
    Pointer<Font> font;

    std::string text = "text";

    Color color = Color::White();
    
    float_t fontSize = 1.f;
};

END_XNOR_CORE

REFL_AUTO(
    type(XnorCore::TextComponent, bases<XnorCore::GuiComponent>),
    field(font),
    field(text),
    field(color),
    field(fontSize)
);
