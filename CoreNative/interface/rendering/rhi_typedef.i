%module CoreNative

%ignore XnorCore::GpuLightData::spotLightSpaceMatrix;
%ignore XnorCore::GpuLightData::dirLightSpaceMatrix;
%ignore XnorCore::SkinnedMeshGpuData;

%include "rendering/rhi_typedef.hpp"
