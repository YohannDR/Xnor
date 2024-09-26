#version 460 core

layout (location = 0) out vec3 gNormal;
layout (location = 1) out vec4 gAlbedoSpec;
layout (location = 2) out vec3 gMetallicRoughessReflectance;
layout (location = 3) out vec2 gAmbiantOcclusion;
layout (location = 4) out vec4 gEmissive;

struct Material
{
    sampler2D albedoMap;
    sampler2D metallicMap;
    sampler2D roughnessMap;
    sampler2D normalMap;
    sampler2D ambiantOcclusionMap;
    sampler2D emissiveMap;
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

in VS_OUT {
    smooth vec4 fragPos;
    smooth vec3 normal;
    smooth vec2 texCoords;

    smooth float metallic;
    smooth float roughness;
    smooth float reflectance;
    smooth float emissive;
    smooth float ambiantOccusion;

    mat3 Tbn;
} fs_in;

uniform Material material;

void main()
{
    if (hasAlbedoMap == false)
    {
        gAlbedoSpec.rgb = albedoColor;
    }
    else
    {
        gAlbedoSpec.rgb = texture(material.albedoMap, fs_in.texCoords).rgb;
    }

    if (hasMetallicMap == false)
    {
        gMetallicRoughessReflectance.r = metallic;
    }
    else
    {
        gMetallicRoughessReflectance.r = texture(material.metallicMap, fs_in.texCoords).r;
    }

    if (hasRoughnessMap == false)
    {
        gMetallicRoughessReflectance.g = roughness;
    }
    else
    {
        gMetallicRoughessReflectance.g = texture(material.roughnessMap, fs_in.texCoords).r;
    }

    if (hasNormalMap == false)
    {
        gNormal = normalize(fs_in.normal);
    }
    else
    {
        // Compute NormalMap
        vec3 normal = texture(material.normalMap, fs_in.texCoords).rgb;
        // Set Normal between 0 and 1
        normal = normal * 2.0f - 1.0f;
        gNormal.rgb = normalize(fs_in.Tbn * normal); 
    }

    float currentOcclusion = 0.f;

    if (hasAmbiantOcclusionMap == false)
    {
        currentOcclusion = ambiantOccusion;
    }
    else
    {
        currentOcclusion = texture(material.ambiantOcclusionMap, fs_in.texCoords).r;
    }

    gMetallicRoughessReflectance = vec3(gMetallicRoughessReflectance.r, gMetallicRoughessReflectance.g, reflectance);
    gAmbiantOcclusion = vec2(currentOcclusion,0);

    if (hasEmisive)
    {
        gEmissive = vec4(texture(material.emissiveMap,fs_in.texCoords).xyz,emissive);
    }
    else
    {
        gEmissive = vec4(emissiveColor,emissive);
    }
}
