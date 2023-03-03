#pragma once
#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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
	void ProcessMesh(aiMesh* mesh, const aiScene* scene);
	void LoadMaterialTextures(const aiMaterial* ai_mat, aiTextureType type, std::vector<Texture>& textures);

	std::string directory_;
	std::vector<Renderable> renderables_;
	std::unordered_set<std::string> texture_name_cache_;
	std::unordered_map<std::string, Material> materials_map_;
};
