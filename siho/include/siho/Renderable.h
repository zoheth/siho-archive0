#pragma once
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
    glm::vec4 base_color_factor=glm::vec4(1.0);
    glm::vec3 specular_color_factor= glm::vec4(0.0);
	glm::vec3 emissive_factor= glm::vec4(0.0);

    float metallic_factor=0.0f;
    float roughness_factor=1.0f;
    float ior=1.5f;

    std::vector<Texture> textures_base_color{};
    std::vector<Texture> textures_roughness{};
    std::vector<Texture> textures_normal{};
    std::vector<Texture> textures_emissive{};
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
        // material_ = new Material;
    }

    void SetMaterialUniforms(UniformObject& uniform_object);

    void Transform(glm::mat4 transform_matrix);

    void setup();

    void render(Shader& shader, UniformObject uniform_object);

private:
    std::vector<Vertex> vertices_;
    std::vector<unsigned int> indices_{};
    Material* material_=nullptr;

    glm::mat4 model_matrix_=glm::mat4(1.0f);

    int texture_slot_=1;
    unsigned int vao_{};
    unsigned int vbo_{};
    unsigned int ebo_{};
};