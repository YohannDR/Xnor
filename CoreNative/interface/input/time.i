%module CoreNative

%import "typemaps/cs_static_class.i"

%cs_static_class(XnorCore::Time);

%typemap(cscode) XnorCore::Time
%{
    public static float TotalTime => GetTotalTime();
    
    public static float LastTotalTime => GetLastTotalTime();
    
    public static float DeltaTime => GetDeltaTime();
    
    public static uint TotalFrameCount => GetTotalFrameCount();
%}

%csmethodmodifiers XnorCore::Time::GetTotalTime "private";
%csmethodmodifiers XnorCore::Time::GetLastTotalTime "private";
%csmethodmodifiers XnorCore::Time::GetDeltaTime "private";
%csmethodmodifiers XnorCore::Time::GetTotalFrameCount "private";

%ignore XnorCore::Time::Update;

%include "input/time.hpp"

%template(GetTotalTime) XnorCore::Time::GetTotalTime<float_t>;
%template(GetLastTotalTime) XnorCore::Time::GetLastTotalTime<float_t>;
%template(GetDeltaTime) XnorCore::Time::GetDeltaTime<float_t>;
%template(GetTotalFrameCount) XnorCore::Time::GetTotalFrameCount<uint32_t>;
