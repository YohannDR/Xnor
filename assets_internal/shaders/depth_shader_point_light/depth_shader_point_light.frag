#version 460 core
layout (location = 0) out float PixelLigthDistance;

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

in vec3 WorldPos;

void main()
{
    //gl_FragDepth = gl_FragCoord.z;
    vec3 pixelToVertex = WorldPos - cameraPos;
    PixelLigthDistance = length(pixelToVertex);
}
