%module CoreNative

%csmethodmodifiers XnorCore::SkinnedMeshRenderer::Begin "protected override";
%csmethodmodifiers XnorCore::SkinnedMeshRenderer::Update "protected override";

%ignore XnorCore::SkinnedMeshRenderer::StartMontage;

%include "scene/component/skinned_mesh_renderer.hpp"
