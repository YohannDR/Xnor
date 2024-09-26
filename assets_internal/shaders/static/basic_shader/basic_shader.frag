#version 460 core
#extension GL_NV_uniform_buffer_std430_layout : enable

out vec4 FragColor;

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

in VS_OUT
{
    vec3 fragPos;
    vec3 normal;
    vec2 texCoords;
} fs_in;

uniform vec3 color;
uniform sampler2D diffuseTexture;

vec3 CalcPointLight(PointLightData light, vec3 viewDir, vec3 fragPos, vec3 normal, vec3 albedo)
{
    float distanceLightToFragment = distance(light.position, fragPos);

    if (distanceLightToFragment > light.radius)
        return vec3(0);

    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (distance * distance);

    vec3 lightColor = light.color * light.intensity;

    // combine results
    vec3 ambient = lightColor * albedo;
    vec3 diffuse = lightColor * diff * albedo;
    vec3 specular = lightColor * spec * albedo;
    
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    
    return ambient + diffuse + specular;
}

vec3 CalcSpotLight(SpotLightData light, vec3 viewDir, vec3 fragPos, vec3 normal, vec3 albedo)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (distance * distance);
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    vec3 lightColor = light.color * light.intensity;

    // combine results
    vec3 ambient = lightColor * albedo;
    vec3 diffuse = lightColor * diff * albedo;
    vec3 specular = lightColor * spec * albedo;
    
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return ambient + diffuse + specular;
}

vec3 CalcDirLight(DirectionalData light,vec3 viewDir,vec3 fragPos, vec3 normal,vec3 albedo)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

    vec3 lightColor = light.color * light.intensity;

    // combine results
    vec3 ambient = lightColor * albedo;
    vec3 diffuse = lightColor * diff * albedo;
    vec3 specular = lightColor * spec * albedo;

    return ambient + diffuse + specular;
}

void main()
{
    vec3 normal = normalize(fs_in.normal);
    vec3 viewDir = normalize(cameraPos - fs_in.fragPos);
    vec3 albedo = vec3(texture(diffuseTexture, fs_in.texCoords));

    vec3 finalColor = CalcDirLight(directionalData, viewDir, fs_in.fragPos, normal, albedo);

    for (int i = 0; i < nbrOfPointLight; i++)
    {
        finalColor += CalcPointLight(pointLightData[i], viewDir, fs_in.fragPos, normal, albedo);
    }

    for (int i = 0; i < nbrOfSpotLight; i++)
    {
        finalColor += CalcSpotLight(spotLightData[i], viewDir, fs_in.fragPos, normal, albedo);
    }

    FragColor = vec4(finalColor, 1);
}
