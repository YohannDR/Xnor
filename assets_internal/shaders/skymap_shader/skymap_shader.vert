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

out VS_OUT
{
    vec3 texCoords;
} vs_out;

void main()
{
    vs_out.texCoords = aPos;

    mat3 viewMat3 = mat3(view);
 
    vec4 pos = projection * mat4(viewMat3) * vec4(aPos, 1.0);

    gl_Position = pos.xyww;
}   
