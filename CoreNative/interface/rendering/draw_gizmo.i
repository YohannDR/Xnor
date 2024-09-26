%module CoreNative

%import "typemaps/cs_static_class.i"

%ignore XnorCore::DrawGizmo::DrawGizmo;
%ignore XnorCore::DrawGizmo::DrawGizmos;

%cs_static_class(XnorCore::DrawGizmo);

%include "rendering/draw_gizmo.hpp"
