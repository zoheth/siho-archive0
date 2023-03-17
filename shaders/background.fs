#version 460 core
out vec4 FragColor;
in vec3 WorldPos;

uniform samplerCube u_EnvironmentMap;

void main()
{	

    vec3 envColor = texture(u_EnvironmentMap, WorldPos).rgb;
    
    // HDR tonemap and gamma correct
    //envColor = envColor / (envColor + vec3(1.0));
    //envColor = pow(envColor, vec3(1.0/2.2)); 
    
    FragColor = vec4(envColor, 1.0);
}