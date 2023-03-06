#version 460 core
in vec3 position;
in vec3 worldNormal;
in vec2 texCoord;
in vec3 viewDir;


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

uniform sampler2D caonima;

uniform Material material;

vec3 specularReflection(vec3 viewDir, vec3 normal, vec3 F0, float roughness);
float distributionGGX(float cosTheta, float roughness);
float geometrySchlickGGX(float cosTheta, float roughness);
vec3 fresnelSchlick(float cosTheta, vec3 F0);

#define PI 3.14159265359

void main() {
    vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0)); // Example directional light direction

    vec4 baseColor = material.baseColorFactor;
    

    if (textureSize(material.baseColorTexture, 0).x > 0) {
        baseColor *= texture(material.baseColorTexture, texCoord);
    }
    
    float metallic = material.metallicFactor;
    float roughness = material.roughnessFactor;
    if (textureSize(material.metallicRoughnessTexture, 0).x > 0) {
        vec4 tex = texture(material.metallicRoughnessTexture, texCoord);
        metallic *= tex.b;
        roughness *= tex.g;
    }

    vec3 emissive = material.emissiveFactor;
    if (textureSize(material.emissiveTexture, 0).x > 0) {
        emissive = texture(material.emissiveTexture, texCoord).rgb;
    }


    vec3 specularColor = material.specularColorFactor;
    float ior = material.ior;

    // Compute lighting
    vec3 diffuse = baseColor.rgb * (1.0 - metallic);

    fragColor = vec4(diffuse,1);
    return;

    vec3 F0 = mix(vec3(0.04), specularColor, metallic);
    vec3 specular = F0 * specularReflection(viewDir, worldNormal, F0, roughness);
    vec3 ambient = 0.02 * baseColor.rgb * diffuse;

    vec3 lighting = diffuse * max(dot(worldNormal, lightDir), 0.0) + specular * max(dot(reflect(-lightDir, worldNormal), viewDir), 0.0) + ambient;

    fragColor = vec4(lighting + emissive, baseColor.a);
}

vec3 specularReflection(vec3 viewDir, vec3 normal, vec3 F0, float roughness) {
    vec3 H = normalize(viewDir + normal);
    float dotNH = max(dot(normal, H), 0.0);
    float d = distributionGGX(dotNH, roughness);
    float G = geometrySchlickGGX(dotNH, roughness);
    vec3 F = fresnelSchlick(max(dot(normal, viewDir), 0.0), F0);
    return (d * G * F) / (4.0 * max(dot(normal, viewDir), 0.0) * max(dot(normal, H), 0.0) + 0.001);
}

float distributionGGX(float cosTheta, float roughness) {
    float alpha = roughness * roughness;
    float alphaSqr = alpha * alpha;
    float cosThetaSqr = cosTheta * cosTheta;
    float denominator = cosThetaSqr * (alphaSqr - 1.0) + 1.0;
    return alphaSqr / (PI * denominator * denominator + 0.001);
}

// 计算 geometry Schlick-GGX 函数
float geometrySchlickGGX(float cosTheta, float roughness) {
    float k = roughness * roughness / 2.0;
    float numerator = cosTheta;
    float denominator = cosTheta * (1.0 - k) + k;
    return numerator / denominator;
}

// 计算 Fresnel Schlick 函数
vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}