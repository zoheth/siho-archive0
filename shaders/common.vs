#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;


out vec3 worldPos;
out vec3 worldNormal;
out vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    worldPos = vec3(model * vec4(aPos, 1.0));
    worldNormal = normalize(mat3(transpose(inverse(model))) * aNormal);
    texCoord = aTexCoords;

    gl_Position = projection * view * vec4(worldPos, 1.0);
}