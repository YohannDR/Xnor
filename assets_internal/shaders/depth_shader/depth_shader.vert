#version 460 core
#extension GL_NV_uniform_buffer_std430_layout : enable


layout (location = 0) in vec3 aPos;

const int MaxSpotLight = 50;
const int MaxPointLight = 50;
const int DirectionalCascadeLevelAllocation = 12;
const int DirectionalCascadeLevel = 4;

const float PI = 3.14159265359;
const float InvPI = 1/PI;

struct PointLightData
{
    vec3 color;
    float intensity;
    vec3 position;
    float radius;
    bool isCastShadow;
};

struct SpotLightData
{
    vec3 color;
    float intensity;
    vec3 position;
    float cutOff;
    vec3 direction;
    float outerCutOff;
    bool isCastShadow;
};

struct DirectionalData
{
    vec3 color;
    float intensity;
    vec3 direction;
    bool isDirlightCastShadow;
    int cascadeCount;
    float cascadePlaneDistance[DirectionalCascadeLevel];
};

layout (std430, binding = 2) uniform LightData
{
    int nbrOfPointLight;
    int nbrOfSpotLight;
    PointLightData pointLightData[MaxPointLight];
    SpotLightData spotLightData[MaxSpotLight];
    DirectionalData directionalData;

    mat4 spothLightlightSpaceMatrix[MaxSpotLight];
    mat4 dirLightSpaceMatrix[DirectionalCascadeLevelAllocation];
    int nbrOfDirLight;
};


layout (std140, binding = 0) uniform CameraUniform
{
    mat4 view;
    mat4 projection;
    mat4 inView;
    mat4 inProjection;
    vec3 cameraPos;
    float near;
    float far;
};

layout (std140, binding = 1) uniform ModelUniform
{
    mat4 model;
    mat4 normalInvertMatrix;
    uint drawId;
};
void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
