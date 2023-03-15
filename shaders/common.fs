#version 460 core
in vec3 worldPos;
in vec3 worldNormal;
in vec2 texCoord;
in mat3 TBN;


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


const float PI = 3.14159265359;

float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 fresnelSchlick(float cosTheta, vec3 F0);

float V_SmithGGXCorrelatedFast(float NoV, float NoL, float roughness);


void main()
{
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
        ao *= tex.a;
    }
    vec3 emissive = u_Material.emissiveFactor;
    if (textureSize(u_Material.emissiveTexture, 0).x > 0) {
        emissive = pow(texture(u_Material.emissiveTexture, texCoord).rgb, vec3(2.2)) * 10;
    }
    vec3 N = normalize(texture(u_Material.normalTexture, texCoord).rgb * 2.0 - 1.0);
    vec3 viewPos = TBN * u_CamPos;
    vec3 fragPos = TBN * worldPos;
    vec3 V = normalize(viewPos - fragPos);

    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    vec3 Lo = vec3(0.0);
    for(int i = 0; i<u_PointLightsCount; ++i)
    {
        vec3 L = TBN * u_PointLights[i].position - fragPos;
        float distance = length(L);
        L = normalize(L);
        vec3 H = normalize(V + L);
        
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = u_PointLights[i].color * attenuation;

        float NDF = DistributionGGX(N, H, roughness);        
        float G   = GeometrySmith(N, V, L, roughness);      
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;

        vec3 numerator    = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
        vec3 specular     = numerator / denominator;

        float NdotL = max(dot(N, L), 0.0);                
        Lo += (kD * albedo / PI + specular) * radiance * NdotL; 
    }
    vec3 ambient = vec3(0.03) * albedo * ao;
    vec3 color = ambient + Lo + emissive;
	
    //color = color / (color + vec3(1.0));
    //color = pow(color, vec3(1.0/2.2));  
   
    fragColor = vec4(color, 1.0);
    float brightness = dot(fragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        brightColor = vec4(fragColor.rgb, 1.0);
    else
        brightColor = vec4(0.0, 0.0, 0.0, 1.0);
}


vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
} 

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    //float a      = roughness*roughness;
    //float a2     = a*a;
    //float NdotH  = max(dot(N, H), 0.0);
    //float NdotH2 = NdotH*NdotH;
	
    //float num   = a2;
    //float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    //denom = PI * denom * denom;
	
    //return num / denom;
    float NdotH  = max(dot(N, H), 0.0);
    float a = NdotH * roughness;
    float k = roughness / (1.0 - NdotH * NdotH + a * a);
    return k * k * (1.0 / PI);
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}

float V_SmithGGXCorrelatedFast(float NoV, float NoL, float roughness) {
    float a = roughness;
    float GGXV = NoL * (NoV * (1.0 - a) + a);
    float GGXL = NoV * (NoL * (1.0 - a) + a);
    return 0.5 / (GGXV + GGXL);
}