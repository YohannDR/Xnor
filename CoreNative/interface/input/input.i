%module CoreNative

%import "typemaps/cs_static_class.i"

%cs_static_class(XnorCore::Input);

%ignore XnorCore::Input::HandleEvents;
%ignore XnorCore::Input::Initialize;
%ignore XnorCore::Input::Update;

%ignore XnorCore::Input::BoundWindowInfo;
%ignore XnorCore::Input::GetBindingId;
%ignore XnorCore::Input::UpdateBoundWindowInfo;
%ignore XnorCore::Input::GetWindowBoundInfo;
%ignore XnorCore::Input::BindWindow;

%include "input/input.hpp"
