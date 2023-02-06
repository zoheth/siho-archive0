#pragma once

#include <map>
#include <vector>
#include <glm/glm.hpp>
#include <assimp/scene.h>

#include <siho/MaterialInstance.h>

class MeshAssimp
{
public:
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
	};
    struct Asset {
        std::string file;
        std::vector<uint32_t> indices;
        std::vector<glm::vec3> positions;
        // std::vector<short4> tangents;
        std::vector<glm::vec2> tex_coords0;
        std::vector<glm::vec2> tex_coords1;
        std::vector<Mesh> meshes;
        std::vector<int> parents;
    };
    void processNode(Asset& asset,
        std::map<std::string, siho::MaterialInstance*>& out_materials,
        const aiScene* scene,
        size_t deep,
        size_t mat_count,
        const aiNode* node,
        int parent_index,
        size_t& depth) const;

};
