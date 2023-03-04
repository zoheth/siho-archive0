#version 460 core
in vec3 normal;
in vec3 frag_pos;
in vec2 tex_coords;


out vec4 FragColor;

struct Material {
    sampler2D texture_diffuse;
    sampler2D texture_specular;
    sampler2D texture_emission;
    sampler2D texture_normal;
    sampler2D texture_height;

    float shininess;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
}; 

uniform Material material;

void main()
{       
    FragColor = texture(material.texture_diffuse, tex_coords)+texture(material.texture_specular, tex_coords);
}