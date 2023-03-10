#version 460 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D u_ScreenTexture0;
uniform sampler2D u_ScreenTexture1;
uniform int u_ScreenTextureCount;
uniform float exposure;

void main()
{             
    const float gamma = 2.2;
    vec3 hdrColor = texture(u_ScreenTexture0, TexCoords).rgb;      
    hdrColor += texture(u_ScreenTexture1, TexCoords).rgb;

    FragColor = vec4(hdrColor, 1.0);
} 