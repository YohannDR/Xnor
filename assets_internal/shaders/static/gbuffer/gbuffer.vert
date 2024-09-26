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

layout (std140, binding = 4) uniform MaterialDataUniform
{
    vec3 albedoColor;
    bool hasAlbedoMap;

    vec3 emissiveColor;
    float emissive;
    bool hasEmisive;

    bool hasMetallicMap;
    float metallic;

    bool hasRoughnessMap;
    float roughness;

    bool hasAmbiantOcclusionMap;
    float ambiantOccusion;

    bool hasNormalMap;
    float reflectance;
};

out VS_OUT
{
    vec4 fragPos;
    vec3 normal;
    vec2 texCoords;

    float metallic;
    float roughness;
    float reflectance;
    float emissive;
    float ambiantOccusion;

    mat3 Tbn;
} vs_out;

void main()
{
    vs_out.fragPos = model * vec4(aPos, 1.0);
    gl_Position = projection * view * vs_out.fragPos ;

    vs_out.texCoords = aTexCoords;
    vs_out.roughness = roughness;
    vs_out.metallic = metallic;
    vs_out.reflectance = reflectance;
    vs_out.emissive = emissive;
    vs_out.ambiantOccusion = ambiantOccusion;

    // Compute Normal
    if (hasNormalMap == false)
    {
        vs_out.normal = mat3(normalInvertMatrix) * aNormal;
    }
    else
    { 
        vs_out.normal = mat3(normalInvertMatrix) * aNormal;
        vec3 T = normalize(vec3(model * vec4(aTangent, 0.0)));
        vec3 B = normalize(vec3(model * vec4(aBitangent, 0.0)));
        vec3 N = normalize(vec3(model * vec4(aNormal, 0.0)));
        vs_out.Tbn = mat3(T, B, N);
    }
}
