#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include <unordered_set>
#include <unordered_map>

#include "Renderable.h"
#include "Texture.h"

class MeshAssimp
{
public:
	void LoadModel(std::string const& path);
	std::vector<Renderable> renderables() { return renderables_; }

private:
	void ProcessNode(const aiNode* node, const aiScene* scene);
	void ProcessMesh(aiMesh* mesh, const aiScene* scene, glm::mat4 transformation);
	void ProcessMaterial(const aiMaterial* material, Material& output_material);
	void LoadMaterialTextures(const aiString ai_str, std::vector<Texture>& textures);

	std::string directory_;
	std::vector<Renderable> renderables_;
	std::unordered_set<std::string> texture_name_cache_;
	std::unordered_map<std::string, Material> materials_map_;
};
