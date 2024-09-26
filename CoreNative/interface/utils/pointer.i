%module CoreNative

%typemap(cscode) XnorCore::Pointer<XnorCore::Mesh>
%{
    public Mesh Value => Get();
%}

%typemap(cscode) XnorCore::Pointer<XnorCore::Texture>
%{
    public Texture Value => Get();
%}

%typemap(cscode) XnorCore::Pointer<XnorCore::Model>
%{
    public Model Value => Get();
%}

%typemap(cscode) XnorCore::Pointer<XnorCore::Animation>
%{
    public Animation Value => Get();
%}

%typemap(cscode) XnorCore::Pointer<XnorCore::AudioTrack>
%{
    public AudioTrack Value => Get();
%}

%typemap(cscode) XnorCore::Pointer<XnorCore::Font>
%{
    public Font Value => Get();
%}

%typemap(cscode) XnorCore::Pointer<XnorCore::Skeleton>
%{
    public Skeleton Value => Get();
%}

%ignore XnorCore::Pointer::GetReferenceCounter;

%ignore XnorCore::Pointer::operator=;
%ignore XnorCore::Pointer::operator*;
%ignore XnorCore::Pointer::operator->;
%ignore XnorCore::Pointer::operator bool_t;

%include "utils/pointer.hpp"

%template(MeshPointer) XnorCore::Pointer<XnorCore::Mesh>;
%template(TexturePointer) XnorCore::Pointer<XnorCore::Texture>;
%template(ModelPointer) XnorCore::Pointer<XnorCore::Model>;
%template(AnimationPointer) XnorCore::Pointer<XnorCore::Animation>;
%template(AudioTrackPointer) XnorCore::Pointer<XnorCore::AudioTrack>;
%template(FontPointer) XnorCore::Pointer<XnorCore::Font>;
%template(SkeletonPointer) XnorCore::Pointer<XnorCore::Skeleton>;
