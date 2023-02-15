#pragma once

#include <map>
#include <vector>
#include <glm/glm.hpp>
#include <assimp/scene.h>

#include <siho/MaterialInstance.h>
#include <siho/Texture.h>
#include <utils/Entity.h>

#include "siho/Engine.h"


struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 tex_coords;
    glm::vec3 tangent;
    glm::vec3 bitangent;
};


class MeshAssimp
{
public:
    explicit MeshAssimp(siho::Engine& engine);
    ~MeshAssimp();

    void addFromFile(const std::string& path,
        std::map<std::string, siho::MaterialInstance*>& materials,
        bool override_material = false);

    const std::vector<utils::Entity> getRenderables() const noexcept {
        return renderables_;
    }

private:
    struct Part {
        size_t offset;
        size_t count;
        std::string material;
        glm::vec3 base_color;
        float opacity;
        float metallic;
        float roughness;
        float reflectance;
    };
	struct Mesh
	{
		size_t offset;
		size_t count;
        std::vector<Part> parts;
        glm::mat4 transform;
        glm::mat4 acc_transform;
	};
    struct Asset {
        std::string file;
        std::vector<uint32_t> indices;
        std::vector<Vertex> vertices;
        std::vector<Mesh> meshes;
        std::vector<int> parents;
    };
    bool setFromFile(Asset& asset, std::map<std::string, siho::MaterialInstance*>& out_materials);

    void processGltfMaterial(const aiScene* scene, const aiMaterial* material,
        const std::string& material_name, const std::string& dir_name,
        std::map<std::string, siho::MaterialInstance*>& out_materials) const;

    void processNode(Asset& asset,
        std::map<std::string, siho::MaterialInstance*>& out_materials,
        const aiScene* scene,
        bool is_gltf,
        size_t deep,
        size_t mat_count,
        const aiNode* node,
        int parent_index,
        size_t& depth) const;

    unsigned int vao_, vbo_, ebo_;
    std::vector<utils::Entity> renderables_;

    std::vector<siho::Texture*> textures_;
};
