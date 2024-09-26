%module CoreNative

%ignore XnorCore::Scene::renderOctree;
%ignore XnorCore::Scene::onDestroyEntity;
%ignore XnorCore::Scene::onCreateEntity;

%include "scene/scene.hpp"
