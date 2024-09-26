%module CoreNative

%csmethodmodifiers XnorCore::Collider::Begin "protected override";
%csmethodmodifiers XnorCore::Collider::Update "protected override";

%ignore XnorCore::Collider::onTriggerEnter;
%ignore XnorCore::Collider::onTriggerStay;
%ignore XnorCore::Collider::onTriggerExit;
%ignore XnorCore::Collider::onCollisionEnter;
%ignore XnorCore::Collider::onCollisionStay;
%ignore XnorCore::Collider::onCollisionExit;

%include "physics/component/collider.hpp"
