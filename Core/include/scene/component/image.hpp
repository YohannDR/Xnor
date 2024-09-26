#pragma once

#include "core.hpp"
#include "gui_component.hpp"
#include "resource/texture.hpp"
#include "utils/pointer.hpp"

BEGIN_XNOR_CORE

class Image : public GuiComponent
{
    REFLECTABLE_IMPL(Image)

public:
    Pointer<Texture> image;
};

END_XNOR_CORE

REFL_AUTO(
    type(XnorCore::Image, bases<XnorCore::GuiComponent>),
    field(image)
);
