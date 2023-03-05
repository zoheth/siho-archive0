#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out vec3 position;
out vec3 worldNormal;
out vec2 texCoord;

out vec3 viewDir;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec3 worldPos = vec3(model * vec4(position, 1.0));
    viewDir = normalize(-vec3(view * vec4(worldPos, 1.0)));

    gl_Position = projection * view * model * vec4(aPos, 1.0);
    position = vec3(model * vec4(aPos, 1.0));
    worldNormal = normalize(mat3(transpose(inverse(model))) * aNormal);
    texCoord = aTexCoords;
}