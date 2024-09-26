#include "scene/component/button.hpp"

#include "input/input.hpp"
#include "scene/entity.hpp"

using namespace XnorCore;

void Button::Update()
{
    Component::Update();
    componentTarget = entity->GetComponent<Image>();
    ButtonUpdate();
}

void Button::ButtonUpdate() const
{
    if (!Input::GetMouseButton(MouseButton::Left, MouseButtonStatus::Pressed))
        return;

    if (componentTarget == nullptr)
        return;

    const Vector2 mousePosApplication = Input::GetMousePosition<Vector2>();
    
    std::vector<Input::BoundWindowInfo> windowBindedInfos;
    Input::GetWindowBoundInfo(&windowBindedInfos);

    // Look for each window if we click on the button bounding box
    for (const Input::BoundWindowInfo& windowInfo : windowBindedInfos)
    {
        if (!windowInfo.isRendering)
            continue;
        
        if (windowInfo.isEditor)
            continue;
        
        const Vector2 mousePosWindow = mousePosApplication - windowInfo.windowPos;

        const Vector2 halfSize = componentTarget->size;
        
        const bool_t xAxis = mousePosWindow.x >= (componentTarget->screenTransform.x - halfSize.x) && 
                             mousePosWindow.x <= (componentTarget->screenTransform.x + halfSize.x);
        
        const bool_t yAxis = mousePosWindow.y >= (componentTarget->screenTransform.y - halfSize.y) &&
                             mousePosWindow.y <= (componentTarget->screenTransform.y + halfSize.y);
        
        if (yAxis && xAxis)
        {
            onClick.Invoke();
        }
    }
}
