%module CoreNative

%include "resource/resource_manager.hpp"

%template(GetMesh) XnorCore::ResourceManager::Get<XnorCore::Mesh>;
%template(GetTexture) XnorCore::ResourceManager::Get<XnorCore::Texture>;
%template(GetModel) XnorCore::ResourceManager::Get<XnorCore::Model>;
%template(GetAnimation) XnorCore::ResourceManager::Get<XnorCore::Animation>;
%template(GetAudioTrack) XnorCore::ResourceManager::Get<XnorCore::AudioTrack>;
%template(GetFont) XnorCore::ResourceManager::Get<XnorCore::Font>;
%template(GetSkeleton) XnorCore::ResourceManager::Get<XnorCore::Skeleton>;
