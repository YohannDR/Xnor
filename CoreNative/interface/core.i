%module(imclassname = "CoreNativeFuncs") CoreNative

%include <csharp/stl.i>
%include <csharp/std_array.i>
%include <csharp/std_unordered_map.i>
%include <csharp/typemaps.i>
%include <csharp/swiginterface.i>
%include <csharp/swigmove.i>
%include <csharp/swigtype_inout.i>
%include <stdint.i>
%include <cpointer.i>

%include <windows.i>

typedef float float_t;

// Make sure the Core and CoreNative classes are partial if we need to manually add something in them
%pragma(csharp) moduleclassmodifiers = "public static partial class";
// The CoreNative class cannot be static as SWIG seems to generate protected members for some reason
%pragma(csharp) imclassclassmodifiers = "internal partial class";

// Rename all C++ operator overloads to intermediate C# functions
%csmethodmodifiers operator "internal";
%rename(OperatorAdd) operator+;
%rename(OperatorSub) operator-;
%rename(OperatorMul) operator*;
%rename(OperatorDiv) operator/;

%rename(OperatorAddAssign) operator+=;
%rename(OperatorSubAssign) operator-=;
%rename(OperatorMulAssign) operator*=;
%rename(OperatorDivAssign) operator/=;

%rename(OperatorEq) operator==;
%rename(OperatorNotEq) operator!=;

%rename(OperatorIndex) operator[];

// Rename string cast operator
%csmethodmodifiers operator std::string "public override";
%rename(ToString) operator std::string;

// Ignore output stream operator
%ignore operator<<;
// Ignore calling operator
%ignore operator();

// These structs are used to copy data between C++ and C# value types
%{
struct Data4
{
    char data[4];
};

struct Data8
{
    char data[8];
};

struct Data12
{
    char data[12];
};

struct Data16
{
    char data[16];
};

struct Data36
{
    char data[36];
};

struct Data64
{
    char data[64];
};
%}

%include "toolbox.i"

%include "core.hpp"

// Ignore these macros
#define DEFAULT_COPY_MOVE_OPERATIONS(...)
#define DELETE_COPY_MOVE_OPERATIONS(...)
#define REFLECTABLE_IMPL(...)
#define REFL_AUTO(...)

%{
#include <format>

#include "csharp/dotnet_utils.hpp"

#include "audio/audio_buffer.hpp"
#include "audio/component/audio_listener.hpp"
#include "audio/component/audio_source.hpp"

#include "file/directory.hpp"
#include "file/entry.hpp"
#include "file/file.hpp"
#include "file/file_manager.hpp"

#include "input/gamepad_input.hpp"
#include "input/input.hpp"
#include "input/keyboard_input.hpp"
#include "input/mouse_input.hpp"
#include "input/time.hpp"

#include "rendering/buffer/uniform_buffer.hpp"

#include "rendering/light/directional_light.hpp"
#include "rendering/light/light.hpp"
#include "rendering/light/point_light.hpp"
#include "rendering/light/spot_light.hpp"

#include "rendering/render_systems/light_manager.hpp"
#include "rendering/render_systems/skybox_renderer.hpp"
#include "rendering/render_systems/tone_mapping.hpp"

#include "rendering/camera.hpp"
#include "rendering/frame_buffer.hpp"
#include "rendering/material.hpp"
#include "rendering/render_pass.hpp"
#include "rendering/renderer.hpp"
#include "rendering/rhi.hpp"
#include "rendering/rhi_typedef.hpp"
#include "rendering/vertex.hpp"
#include "rendering/viewport.hpp"
#include "rendering/viewport_data.hpp"
#include "rendering/draw_gizmo.hpp"
#include "utils/plane.hpp"

#include "physics/data/collision_data.hpp"

#include "physics/component/collider.hpp"
#include "physics/component/box_collider.hpp"
#include "physics/component/capsule_collider.hpp"
#include "physics/component/mesh_collider.hpp"
#include "physics/component/sphere_collider.hpp"

#include "resource/font.hpp"
#include "resource/audio_track.hpp"
#include "resource/animation_montage.hpp"
#include "resource/model.hpp"
#include "resource/texture.hpp"
#include "resource/animation.hpp"
#include "resource/mesh.hpp"
#include "resource/resource.hpp"
#include "resource/resource_manager.hpp"
#include "resource/shader.hpp"

#include "scene/component/camera_component.hpp"
#include "scene/component/static_mesh_renderer.hpp"
#include "scene/component/skinned_mesh_renderer.hpp"
#include "scene/component/script_component.hpp"

#include "scene/component.hpp"
#include "scene/entity.hpp"
#include "scene/scene.hpp"

#include "serialization/serializer.hpp"

#include "utils/color.hpp"
#include "utils/formatter.hpp"
#include "utils/guid.hpp"
#include "utils/event.hpp"
#include "utils/list.hpp"
#include "utils/logger.hpp"
#include "utils/meta_programming.hpp"
#include "utils/pointer.hpp"
#include "utils/reference_counter.hpp"
#include "utils/ts_queue.hpp"
#include "utils/bound.hpp"
#include "utils/utils.hpp"

#include "world/scene_graph.hpp"
#include "world/skybox.hpp"
#include "world/world.hpp"

#include "application.hpp"
#include "core.hpp"
#include "screen.hpp"
#include "transform.hpp"
#include "window.hpp"
%}

%include "utils/logger.i"
%include "utils/pointer.i"
%include "utils/guid.hpp"
%include "utils/list.i"
%include "utils/color.i"

%include "rendering/draw_gizmo.i"

%include "resource/resource.hpp"

%include "file/entry.hpp"
%include "file/directory.hpp"
%include "file/file.i"
%include "file/file_manager.hpp"

%include "rendering/vertex.hpp"
%include "rendering/rhi_typedef.i"

%include "resource/texture.hpp"
%include "resource/font.hpp"

%include "transform.i"

%include "utils/plane.hpp"

%include "utils/bound.hpp"

%include "resource/model.hpp"

%include "resource/texture.hpp"
%include "rendering/material.hpp"
%include "rendering/bone.hpp"

%include "resource/skeleton.hpp"
%include "resource/animation.hpp"

%include "resource/mesh.hpp"
%include "resource/resource_manager.i"

%include "resource/shader.hpp"

%include "resource/mesh.hpp"

%include "input/gamepad_input.hpp"
%include "input/keyboard_input.hpp"
%include "input/mouse_input.hpp"
%include "input/input.i"
%include "input/time.i"

%include "scene/component.i"

%include "csharp/dotnet_utils.hpp"

%include "rendering/light/light.hpp"
%include "rendering/light/directional_light.hpp"
%include "rendering/light/point_light.hpp"
%include "rendering/light/spot_light.hpp"

%include "rendering/camera.hpp"

%include "scene/component/camera_component.i"

%include "rendering/material.hpp"

%include "audio/audio_buffer.hpp"

%include "resource/audio_track.hpp"

%include "audio/component/audio_listener.i"
%include "audio/component/audio_source.i"

%include "physics/data/collision_data.hpp"

%include "physics/component/collider.i"
%include "physics/component/box_collider.i"
%include "physics/component/capsule_collider.i"
%include "physics/component/mesh_collider.i"
%include "physics/component/sphere_collider.i"

%include "scene/component/static_mesh_renderer.hpp"
%include "scene/component/skinned_mesh_renderer.i"
%include "scene/component/script_component.i"

%include "scene/entity.i"

%include "world/skybox.hpp"

%include "scene/scene.i"
%include "utils/utils.hpp"

%include "world/scene_graph.hpp"
%include "world/world.hpp"

%include "screen.hpp"
%include "window.hpp"
