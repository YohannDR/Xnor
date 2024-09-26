#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;  

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


out vec4 Color;

void main()
{
    gl_Position =  projection * view * model * vec4(aPos, 1.0);
}