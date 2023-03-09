#version 460 core
in vec3 worldPos;
in vec3 worldNormal;
in vec2 texCoord;


out vec4 fragColor;

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

uniform Material material;
uniform PointLight pointLights[4];
uniform int pointLightsCount;
uniform vec3 camPos;


const float PI = 3.14159265359;

float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 fresnelSchlick(float cosTheta, vec3 F0);


void main()
{
    vec3 albedo     = material.baseColorFactor.rgb;
    if(textureSize(material.baseColorTexture, 0).x > 0) 
    {
        albedo     *= pow(texture(material.baseColorTexture, texCoord).rgb, vec3(2.2));
    }
    float metallic  = material.metallicFactor;
    float roughness = material.roughnessFactor;
    float ao = 1.0;
    if (textureSize(material.metallicRoughnessTexture, 0).x > 0) {
        vec4 tex = texture(material.metallicRoughnessTexture, texCoord);
        metallic *= tex.b;
        roughness *= tex.g;
        ao *= tex.a;
    }
    vec3 emissive = material.emissiveFactor;
    if (textureSize(material.emissiveTexture, 0).x > 0) {
        emissive = texture(material.emissiveTexture, texCoord).rgb;
    }
    
    vec3 V = normalize(camPos - worldPos);

    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    vec3 Lo = vec3(0.0);
    for(int i = 0; i<pointLightsCount; ++i)
    {
        vec3 L = normalize(pointLights[i].position - worldPos);
        vec3 H = normalize(V + L);

        float distance = length(pointLights[i].position - worldPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = pointLights[i].color * attenuation;

        float NDF = DistributionGGX(worldNormal, H, roughness);        
        float G   = GeometrySmith(worldNormal, V, L, roughness);      
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;

        vec3 numerator    = NDF * G * F;
        float denominator = 4.0 * max(dot(worldNormal, V), 0.0) * max(dot(worldNormal, L), 0.0) + 0.0001;
        vec3 specular     = numerator / denominator;

        float NdotL = max(dot(worldNormal, L), 0.0);                
        Lo += (kD * albedo / PI + specular) * radiance * NdotL; 
    }
    vec3 ambient = vec3(0.03) * albedo * ao;
    vec3 color = ambient + Lo + emissive;
	
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));  
   
    fragColor = vec4(color, 1.0);


}


vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
} 

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom;
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