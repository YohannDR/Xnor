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


uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D gMetallicRoughessReflectance;
uniform sampler2D gAmbiantOcclusion;
uniform sampler2D gEmissive;
uniform sampler2D gDepth;

uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D brdfLUT;

uniform sampler2DArray dirLightShadowMap;
uniform sampler2DArray spotLightShadowArray;

uniform samplerCubeArray pointLightCubemapArrayPixelDistance;

in vec2 texCoords;

const vec3 gridSamplingDiskVec3[20] = vec3[]
(
    vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1),
    vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
    vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
    vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
    vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

const vec2 gridSamplingDiskVec2[20] = vec2[]
(
    vec2(1, 1), vec2( 1, -1), vec2(-1, -1), vec2(-1, 1),
    vec2(1, 1), vec2( 1, -1), vec2(-1, -1), vec2(-1, 1),
    vec2(1, 1), vec2( 1, -1), vec2(-1, -1), vec2(-1, 1),
    vec2(1, 0), vec2(-1,  0), vec2( 1,  0), vec2(-1, 0),
    vec2(0, 1), vec2( 0, -1), vec2( 0, -1), vec2( 0, 1)
);


// this is supposed to get the world position from the depth buffer
vec4 WorldPosFromDepth(float depth) {
    float z = depth * 2.0 - 1.0;

    vec4 clipSpacePosition = vec4(texCoords * 2.0 - 1.0, z, 1.0);
    vec4 viewSpacePosition = inProjection * clipSpacePosition;

    // Perspective division
    viewSpacePosition /= viewSpacePosition.w;

    vec4 worldSpacePosition = inView * viewSpacePosition;

    return worldSpacePosition;
}

float DirLightShadowCalculation(vec4 fragPosWorldSpace, vec3 n, vec3 l)
{
    // select cascade layer
    vec4 fragPosViewSpace = view * vec4(fragPosWorldSpace.xyz, 1.0);
    float depthValue = abs(fragPosViewSpace.z);

    int layer = -1;
    
    for (int i = 0; i < directionalData.cascadeCount; ++i)
    {
        if (depthValue < directionalData.cascadePlaneDistance[i])
        {
            layer = i;
            break;
        }
    }
    if (layer == -1)
    {
        layer = directionalData.cascadeCount;
    }
    
    vec4 fragPosLightSpace = dirLightSpaceMatrix[layer] * vec4(fragPosWorldSpace.xyz, 1.0);
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;

    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if (currentDepth > 1.0)
    {
        return 0.0;
    }
    float bias = max(0.01 * (1.0 - dot(n, l)), 0.001);    
    // calculate bias (based on depth map resolution and slope)
    const float biasModifier = 0.9f;
    if (layer == directionalData.cascadeCount)
    {
        bias *= 1 / (far * biasModifier);
    }
    else
    {
        bias *= 1 / (directionalData.cascadePlaneDistance[layer] * biasModifier );
    }

    // PCF  
    float shadow = 0.0;
    vec2 texelSize = 1.0 / vec2(textureSize(dirLightShadowMap, 0));
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(dirLightShadowMap, vec3(projCoords.xy + vec2(x, y) * texelSize, layer)).r;

            shadow += (currentDepth - bias) > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0f;

    return shadow;
}

float ShadowCalculationSpolight(vec4 fragPosLightSpace, vec3 n, vec3 l, int index)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(spotLightShadowArray, vec3(projCoords.xy, index)).r; // Corrected indexing
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(n);
    vec3 lightDir = normalize(l);
    float bias = max(0.00005 * (1.0 - dot(normal, lightDir)), 0.0000005f);
    // check whether current frag pos is in shadow
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
    // PCF
    vec2 texelSize = 1.0 / textureSize(spotLightShadowArray, 0).xy;
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            vec2 texel = projCoords.xy + vec2(x, y) * texelSize;
            float pcfDepth = texture(spotLightShadowArray, vec3(texel, index)).r; // Corrected indexing
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
    shadow = 0.0;
    
    return shadow;
}

float ShadowCalculationPointLight(vec3 LightToPixel, vec3 fragPos, int index)
{
    float currentDistance = length(LightToPixel);
    
    float shadow = 0.0;
    float bias = 0.20;
    int samples = 20;
    float viewDistance = length(cameraPos - fragPos);
    float diskRadius = (1.0 + (viewDistance / far)) / 25.0;

    for(int i = 0; i < samples; ++i)
    {
        vec3 offset = gridSamplingDiskVec3[i] * diskRadius;
        vec3 textureUv = (LightToPixel + offset) * far;
        float distance = texture(pointLightCubemapArrayPixelDistance, vec4(textureUv.x, textureUv.y, textureUv.z , index)).r;
        
        if(distance + bias < currentDistance)
            shadow += 1.0;
    }
    shadow /= float(samples);
    
    return shadow;
}

//https://de45xmedrsdbp.cloudfront.net/Resources/files/2013SiggraphPresentationsNotes-26915738.pdf
// reparameterization of α = Roughness^2
float SpecularD(float NoH,float a)
{
    float numerator = a * a;
    float denominator = PI * pow( pow(NoH,2) * (numerator - 1) + 1,2);
        
    return numerator / denominator;
}

float SpecularG1(vec3 n,vec3 v,float k)
{
    float NoV = clamp(dot(n,v), 0.0, 1.0);
    
    return NoV / ( NoV * ( 1 - k) + k );
}

float SpecularG(vec3 l, vec3 v, vec3 h, vec3 n, float roughness)
{
    float k = pow((roughness + 1.0), 2.0) / 8.0;

    return SpecularG1(n,l,k) * SpecularG1(n,v,k);
}
vec3 SpecularF(float VoH,vec3 F0, float roughness)
{
    float power = (-5.5473 * VoH - 6.98316) * VoH;
    return F0 + ((1 - F0) - roughness) * pow(2,power); 
}

vec3 ComputeIbl(float roughness,vec3 kD, float ao,vec3 albedo, vec3 N , vec3 R ,vec3 V,vec3 F) 
{
    vec3 irradiance = texture(irradianceMap, N).rgb;
    vec3 diffuse    = irradiance * albedo;
    const float MAX_REFLECTION_LOD = 4.0;
    vec3 prefilteredColor = textureLod(prefilterMap, R,  roughness * MAX_REFLECTION_LOD).rgb;
    vec2 brdf  = texture(brdfLUT, vec2(max(dot(N, V), 0.0), roughness)).rg;
    vec3 specularAmbiant = prefilteredColor * (F * brdf.x + brdf.y);

    vec3 ambient = (kD * diffuse + specularAmbiant) * ao;

    return ambient;
}
    

vec3 ComputeSpotLight(vec3 baseColor,vec4 fragPos,vec3 v, vec3 n, float roughness, float metallic, vec3 f0)
{
    vec3 outLo = vec3(0.f);
    vec3 fragposVec3 = vec3(fragPos.x, fragPos.y, fragPos.z);

    for (int i = 0; i < nbrOfSpotLight; i++)
    {
        SpotLightData light = spotLightData[i];
        float distance = length(light.position - fragposVec3);
        
        vec3 l = normalize(vec3(light.position - fragposVec3));
        vec3 h = normalize(v + l);

        float theta = dot(l, normalize(-light.direction));
        float epsilon = light.cutOff - light.outerCutOff;
        float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);


        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = light.color * attenuation * light.intensity * intensity;
        
        float NoH = clamp(dot(n,h),0.0,1.0);
        float VoH = clamp(dot(v,h),0.0,1.0);
        

        float ndf = SpecularD(NoH, roughness);
        float g =  SpecularG(l, v, h, n, roughness);
        vec3 f = SpecularF(VoH, f0, roughness);

        vec3 numerator = ndf * g * f;
        float denominator = 4.0 * max(dot(n, v), 0.0) * max(dot(n, l), 0.0) + 0.0001;
        
        vec3 specular  = numerator / denominator;
        vec3 kS = f;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;
        float NdotL = max(dot(n, l), 0.0);
        
        NdotL = max(dot(n, l), 0.0);
        vec3 Lo = (kD * baseColor * InvPI + specular) * radiance * NdotL;
        
        if (light.isCastShadow) 
        {
            float shadow = ShadowCalculationSpolight(spothLightlightSpaceMatrix[i] * fragPos, n, l, i);
            Lo *= ( 1.0 - shadow );
        }
        outLo += Lo;
    }
    
    return outLo;
}

vec3 ComputePointLight(vec3 baseColor,vec3 fragPos,vec3 v, vec3 n, float roughness, float metallic, vec3 f0)
{
    vec3 outLo = vec3(0.f);

    for (int i = 0; i < nbrOfPointLight; i++)
    {
        PointLightData light = pointLightData[i];
        float distance = length(light.position - fragPos);

        if(distance > light.radius)
        continue;

        vec3 l = normalize(vec3(light.position - fragPos));
        vec3 h = normalize(v + l);

        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = light.color * attenuation * light.intensity;

        float NoH = clamp(dot(n,h),0.0,1.0);
        float VoH = clamp(dot(v,h),0.0,1.0);


        float ndf = SpecularD(NoH, roughness);
        float g =  SpecularG(l, v, h, n, roughness);
        vec3 f = SpecularF(VoH,f0,roughness);

        vec3 numerator = ndf * g * f;
        float denominator = 4.0 * max(dot(n, v), 0.0) * max(dot(n, l), 0.0) + 0.0001;

        vec3 specular  = numerator / denominator;
        vec3 kS = f;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;
        float NdotL = max(dot(n, l), 0.0);

        
        NdotL = max(dot(n, l), 0.0);
        vec3 Lo = (kD * baseColor * InvPI + specular) * radiance * NdotL;

        if (light.isCastShadow)
        {
            float shadow = ShadowCalculationPointLight(fragPos - light.position, fragPos, i);
            Lo *= ( 1.0 - shadow );
        }
        outLo += Lo;
    }

    return outLo;
}




void main()
{
    vec4 fragPosVec4 = WorldPosFromDepth(texture(gDepth,texCoords).r);
    vec3 fragPos = fragPosVec4.xyz;

    vec3 normal = texture(gNormal, texCoords).rgb;
    vec3 albedo =  pow(texture(gAlbedoSpec, texCoords).rgb,vec3(2.2));
    
    float metallic = texture(gMetallicRoughessReflectance,texCoords).r;
    float roughness = texture(gMetallicRoughessReflectance,texCoords).g;
    float reflectance = texture(gMetallicRoughessReflectance,texCoords).b;

    float ambientOcclusion = texture(gAmbiantOcclusion,texCoords).r;

    vec4 emissiveValue = texture(gEmissive,texCoords);
    
    float emissive = emissiveValue.w;
    vec3 emissiveColor = emissiveValue.rgb;

    vec3 F0 = vec3(0.04);
    F0 = 0.16 * reflectance * reflectance * (1.0 - metallic) + albedo * metallic;
    
    vec3 Lo = vec3(0.0);
    
    // view unit vector
    vec3 v = normalize(cameraPos - fragPos);
    // Surface Normal
    vec3 n = normalize(normal);
    //reflect Vector
    vec3 r = reflect(-v, n); 

    // Incident Light Unit Vector
    vec3 l = normalize(directionalData.direction);
    // half unit vector
    vec3 h = normalize(v + l);
    
    float NoH = clamp(dot(n,h),0.0,1.0);
    float VoH = clamp(dot(v,h),0.0,1.0);
    
    float ndf = SpecularD(NoH, roughness * roughness );
    float g =  SpecularG(l, v, h, n, roughness);
    vec3 f = SpecularF(VoH,F0,roughness);

    vec3 kD = vec3(0,0,0);
    
    if (nbrOfDirLight == 1)
    {
        vec3 radiance = directionalData.color * directionalData.intensity;
        vec3 numerator = ndf * g * f;
        float denominator = 4.0 * max(dot(n, v), 0.0) * max(dot(n, l), 0.0) + 0.0001;
        vec3 specular  = numerator / denominator;

        vec3 kS = f;
        kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;
        float NdotL = max(dot(n, l), 0.0);

        vec3 LoDir = (kD * albedo * InvPI + specular) * radiance * NdotL;
        if (directionalData.isDirlightCastShadow)
        {
            float shadow = DirLightShadowCalculation(fragPosVec4, n, l);
            LoDir *= (1.0-shadow);
        }
        Lo += LoDir;
    }
    
    Lo += ComputePointLight(albedo, fragPos, v, n, roughness, metallic, F0);
    Lo += ComputeSpotLight(albedo, fragPosVec4, v, n, roughness, metallic, F0);
    
    vec3 ambient = ComputeIbl(roughness, kD, ambientOcclusion, albedo, n, r, v, f);
    vec3 color = Lo + ambient + (emissiveColor * emissive);
 
    FragColor = vec4(color, 1);
    
}
