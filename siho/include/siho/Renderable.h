#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include <utility>
#include <vector>

#include "Texture.h"
#include "Shader.h"
#include "UniformObject.h"

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;
};

struct Material {
    std::string name;
    glm::vec4 ambient;
    glm::vec4 diffuse;
    glm::vec4 specular;
    float shininess;
    std::vector<Texture> textures_diffuse;
    std::vector<Texture> textures_specular;
    std::vector<Texture> textures_normal;
    std::vector<Texture> textures_height;
};

class Renderable {
public:
    Renderable(std::vector<Vertex> vertices, std::vector<unsigned int> indices, Material* material) :
		vertices_(std::move(vertices)), indices_(std::move(indices)), material_(material)
    {
        setup();
    }

    void SetMaterialUniforms(UniformObject& uniform_object);

    void setup();

    void render(Shader& shader);

private:
    std::vector<Vertex> vertices_;
    std::vector<unsigned int> indices_;
    Material* material_;

    unsigned int texture_slot_{};
    unsigned int vao_{};
    unsigned int vbo_{};
    unsigned int ebo_{};
};