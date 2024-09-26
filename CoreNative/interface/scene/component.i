%module CoreNative

%typemap(csclassmodifiers) XnorCore::Component "public abstract class";

%typemap(cscode) XnorCore::Component
%{
  public virtual void Awake() {}

  protected virtual void Begin() {}

  protected virtual void Update() {}

  public virtual void PrePhysics() {}

  public virtual void PostPhysics() {}

  public virtual void OnRendering() {}

  public virtual void Destroy() {}
%}

%ignore XnorCore::Component::Awake;
%ignore XnorCore::Component::Begin;
%ignore XnorCore::Component::Update;
%ignore XnorCore::Component::PrePhysics;
%ignore XnorCore::Component::PostPhysics;
%ignore XnorCore::Component::OnRendering;
%ignore XnorCore::Component::Destroy;

%rename(Entity) XnorCore::Component::entity;

%include "scene/component.hpp"
