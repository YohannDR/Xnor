#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;
layout (location = 5) in vec4 aBoneIndices;
layout (location = 6) in vec4 aBoneWeights;
#define MaxBones 100


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


layout (std140, binding = 5) uniform SkinnedBuffer
{
    mat4 mat[MaxBones];
};

layout (std140, binding = 1) uniform ModelUniform
{
    mat4 model;
    mat4 normalInvertMatrix;
    uint drawId;
};


void main()
{
    mat3 rotMatrix = mat3(0.0f);
    vec4 finalPosition = vec4(0.0, 0.0, 0.0, 1.0);

    for(int i = 0; i < 4; i++)
    {
        int idx = int(aBoneIndices[i]);
        if (idx == -1)
            continue;

        if (idx >= MaxBones)
        {
            finalPosition = vec4(aPos, 1.0f);
            break;
        }

        vec4 localPosition = mat[idx] * vec4(aPos ,1.0f);
        finalPosition += localPosition * aBoneWeights[i];
    }

    // Set the fragement pose base on animation and the model matrix
    gl_Position = projection * view *  model * vec4(finalPosition.xyz, 1.0);
}
