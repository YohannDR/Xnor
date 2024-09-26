#pragma once

#include "definitions.hpp"
#include "input/input.hpp"
#include "rendering/camera.hpp"
#include "scene/entity.hpp"

BEGIN_XNOR_EDITOR

struct EditorData
{
    XnorCore::Entity* selectedEntity = nullptr;
    bool_t gotoObject = false;
    XnorCore::Pointer<XnorCore::File> currentScene;
    
    XnorCore::Camera editorCam;
    XnorCore::Viewport editorViewPort;
};

END_XNOR_EDITOR
