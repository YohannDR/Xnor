#include "..\..\include\rendering\post_process_render_target.hpp"

using namespace XnorCore;


void PostProcessRenderTarget::Init(Vector2i viewportSize)
{
    bloomRenderTarget.Init(viewportSize);
}

void PostProcessRenderTarget::Destroy()
{
    bloomRenderTarget.Destroy();
}
