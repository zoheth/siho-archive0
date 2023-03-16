#version 460 core
in vec3 worldPos;
in vec3 worldNormal;
in vec2 texCoord;
//in mat3 TBN;


layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec4 brightColor;

struct Material {
    vec4 baseColorFactor;
    sampler2D baseColorTexture;
    float metallicFactor;
    float roughnessFactor;
    sampler2D metallicRoughnessTexture;
    sampler2D normalTexture;
    vec3 emissiveFactor;
    sampler2D emissiveTexture;
    vec3 specularColorFactor;
    float ior;
};

struct PointLight {
    vec3 position;
    vec3 color;
    float intensity;
};

uniform Material u_Material;
uniform PointLight u_PointLights[4];
uniform int u_PointLightsCount;
uniform vec3 u_CamPos;

uniform samplerCube u_IrradianceMap;
uniform samplerCube u_PrefilterMap;
uniform sampler2D u_BrdfLUT;

const float PI = 3.14159265359;

vec3 F_Schlick(float u, vec3 f0);
vec3 BRDF(vec3 baseColor, vec3 n, vec3 v, vec3 l, vec3 f0, float perceptualRoughness, float metallic);


vec3 GetNormalFromMap()
{
    vec3 tangentNormal = texture(u_Material.normalTexture, texCoord).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(worldPos);
    vec3 Q2  = dFdy(worldPos);
    vec2 st1 = dFdx(texCoord);
    vec2 st2 = dFdy(texCoord);

    vec3 N   = normalize(worldNormal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

void main()
{
    //fragColor = vec4(u_CamPos, 1.0);
    //return;
    vec3 albedo     = u_Material.baseColorFactor.rgb;
    if(textureSize(u_Material.baseColorTexture, 0).x > 0) 
    {
        albedo     *= pow(texture(u_Material.baseColorTexture, texCoord).rgb, vec3(2.2));
    }
    float metallic  = u_Material.metallicFactor;
    float roughness = u_Material.roughnessFactor;
    float ao = 1.0;
    if (textureSize(u_Material.metallicRoughnessTexture, 0).x > 0) {
        vec4 tex = texture(u_Material.metallicRoughnessTexture, texCoord);
        metallic *= tex.b;
        roughness *= tex.g;
        ao *= tex.r;
    }
    vec3 emissive = u_Material.emissiveFactor;
    if (textureSize(u_Material.emissiveTexture, 0).x > 0) {
        emissive = pow(texture(u_Material.emissiveTexture, texCoord).rgb, vec3(2.2)) * 10;
    }

    //vec3 N = normalize(texture(u_Material.normalTexture, texCoord).rgb * 2.0 - 1.0);
    vec3 N = GetNormalFromMap();
    vec3 viewPos = u_CamPos;
    vec3 fragPos = worldPos;
    vec3 V = normalize(viewPos - fragPos);

    //fragColor = vec4(V, 1.0);
    //return;

    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    vec3 Lo = vec3(0.0);
    for(int i = 0; i<u_PointLightsCount; ++i)
    {
        vec3 L = u_PointLights[i].position - fragPos;
        float distance = length(L);
        L = normalize(L);
        
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = u_PointLights[i].color * attenuation;

        vec3 F0 = vec3(0.04); 
        F0 = mix(F0, albedo, metallic);

        float NdotL = max(dot(N, L), 0.0);    
        vec3 brdf = BRDF(albedo,N,V,L,F0,roughness,metallic);            
        Lo += brdf * radiance * NdotL; 
    }

    vec3 kS = F_Schlick(max(dot(N, V), 0.0), F0);
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;
    vec3 irradiance = texture(u_IrradianceMap, worldNormal).rgb;
    vec3 diffuse = irradiance * albedo;

    const float MAX_REFLECTION_LOD = 4.0;
    vec3 R = reflect(-V, N); 
    vec3 prefilteredColor = textureLod(u_PrefilterMap, R,  roughness * MAX_REFLECTION_LOD).rgb;    
    vec2 brdf  = texture(u_BrdfLUT, vec2(max(dot(N, V), 0.0), roughness)).rg;
    vec3 specular = prefilteredColor * (kS * brdf.x + brdf.y);
    
    vec3 ambient = (kD * diffuse + specular) * ao;



    vec3 color = ambient  + Lo + emissive;

    //color = color / (color + vec3(1.0));
    //color = pow(color, vec3(1.0/2.2));  
   
    fragColor = vec4(color, 1.0);
    float brightness = dot(fragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        brightColor = vec4(fragColor.rgb, 1.0);
    else
        brightColor = vec4(0.0, 0.0, 0.0, 1.0);
}



float D_GGX(float NoH, float roughness) {
    float a = NoH * roughness;
    float k = roughness / (1.0 - NoH * NoH + a * a);
    return k * k * (1.0 / PI);
}

vec3 F_Schlick(float u, vec3 f0) {
    return f0 + (vec3(1.0) - f0) * pow(1.0 - u, 5.0);
}

float F_Schlick_f(float u, float f0, float f90) {
    return f0 + (f90 - f0) * pow(1.0 - u, 5.0);
}

float V_SmithGGXCorrelated(float NoV, float NoL, float roughness) {
    float a = roughness;
    float GGXV = NoL * (NoV * (1.0 - a) + a);
    float GGXL = NoV * (NoL * (1.0 - a) + a);
    return 0.5 / (GGXV + GGXL);
}

float Fd_Lambert() {
    return 1.0 / PI;
}
float Fd_Burley(float NoV, float NoL, float LoH, float roughness) {
    float f90 = 0.5 + 2.0 * roughness * LoH * LoH;
    float lightScatter = F_Schlick_f(NoL, 1.0, f90);
    float viewScatter = F_Schlick_f(NoV, 1.0, f90);
    return lightScatter * viewScatter * (1.0 / PI);
}


vec3 BRDF(vec3 baseColor, vec3 n, vec3 v, vec3 l, vec3 f0, float perceptualRoughness, float metallic) {
    vec3 h = normalize(v + l);

    float NoV = abs(dot(n, v)) + 1e-5;
    float NoL = clamp(dot(n, l), 0.0, 1.0);
    float NoH = clamp(dot(n, h), 0.0, 1.0);
    float LoH = clamp(dot(l, h), 0.0, 1.0);

    // perceptually linear roughness to roughness (see parameterization)
    float roughness = perceptualRoughness * perceptualRoughness;
    //float roughness = perceptualRoughness;

    float D = D_GGX(NoH, roughness);
    vec3  F = F_Schlick(LoH, f0);

    float V = V_SmithGGXCorrelated(NoV, NoL, roughness);
    V=clamp(V,0.0,1.0);

    // specular BRDF
    vec3 Fr = (D * V) * F;
    //Fr=vec3(V);
    // diffuse BRDF
    vec3 kD = vec3(1.0) - F;
    kD *= 1.0 - metallic;
    //vec3 Fd = baseColor * Fd_Lambert() * kD;
    vec3 Fd = baseColor * Fd_Burley(NoV,NoL,LoH,roughness) * kD;

    return Fr+Fd;
}