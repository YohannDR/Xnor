#pragma once
#include "core.hpp"
#include "definitions.hpp"
#include "render_window.hpp"

BEGIN_XNOR_EDITOR
class GameWindow : public RenderWindow
{
public:
    using RenderWindow::RenderWindow;
    
    void FetchInfo() override;
};

END_XNOR_EDITOR
