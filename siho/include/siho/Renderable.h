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
    glm::vec3 Normal=glm::vec3(0.0f,0.0f,1.0f);
    glm::vec2 TexCoords=glm::vec3(0.0f);
    glm::vec3 Tangent=glm::vec3(1.0f,0.0f,0.0f);
    glm::vec3 Bitangent=glm::vec3(0.0f,1.0f,0.0f);
};

struct Material {
    std::string name;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess=32;
    std::vector<Texture> textures_diffuse{};
    std::vector<Texture> textures_specular{};
    std::vector<Texture> textures_normal{};
    std::vector<Texture> textures_height{};
};

class Renderable {
public:
    Renderable(std::vector<Vertex> vertices, std::vector<unsigned int> indices, Material* material) :
		vertices_(std::move(vertices)), indices_(std::move(indices)), material_(material)
    {
        setup();
    }

    explicit Renderable(std::vector<Vertex> vertices, glm::vec3 color):
		vertices_(std::move(vertices))
    {
        setup();
        material_ = new Material;
        material_->specular=color;
        material_->diffuse = 0.8f * color;
        material_->ambient = 0.2f * color;

    }

    void SetMaterialUniforms(UniformObject& uniform_object);

    void setup();

    void render(Shader& shader);

private:
    std::vector<Vertex> vertices_;
    std::vector<unsigned int> indices_{};
    Material* material_=nullptr;

    unsigned int texture_slot_{};
    unsigned int vao_{};
    unsigned int vbo_{};
    unsigned int ebo_{};
};