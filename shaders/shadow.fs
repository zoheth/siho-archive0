#version 460 core
in vec4 FragPos;

uniform vec3 u_LightPos;

float far_plane = 25.0f;

void main()
{
    // get distance between fragment and light source
    float lightDistance = length(FragPos.xyz - u_LightPos);
    
    // map to [0;1] range by dividing by far_plane
    lightDistance = lightDistance / far_plane;
    
    // write this as modified depth
    gl_FragDepth = lightDistance;
}  