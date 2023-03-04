#include <siho/MeshAssimp.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <siho/Texture.h>

#define STB_IMAGE_IMPLEMENTATION
#include <iostream>

#include "stb_image.h"

void MeshAssimp::LoadModel(std::string const& path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return;
	}
    directory_ = path.substr(0, path.find_last_of('\\'));

	ProcessNode(scene->mRootNode, scene);
}

void MeshAssimp::ProcessNode(const aiNode* node, const aiScene* scene)
{
	for (size_t i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		ProcessMesh(mesh, scene);
	}
	for (size_t i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

void MeshAssimp::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<Texture> textures;
	std::vector<unsigned int> indices;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector; 
        // positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;
        // normals
        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
        }
        // texture coordinates
        if (mesh->mTextureCoords[0]) 
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
            // tangent
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.Tangent = glm::normalize(vector);
            // bi tangent
            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            vertex.Bitangent = glm::normalize(vector);
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	std::string material_name;
    if (aiString name; material->Get(AI_MATKEY_NAME, name) != AI_SUCCESS)
    {
        material_name = AI_DEFAULT_MATERIAL_NAME;
    }
    else {
        material_name = name.C_Str();
    }

    if(materials_map_.find(material_name) == materials_map_.end())
    {
        Material mat;
		LoadMaterialTextures(material, aiTextureType_DIFFUSE, mat.textures_diffuse);
		LoadMaterialTextures(material, aiTextureType_SPECULAR, mat.textures_specular);
		LoadMaterialTextures(material, aiTextureType_HEIGHT, mat.textures_normal);
		LoadMaterialTextures(material, aiTextureType_AMBIENT, mat.textures_height);
        aiColor4D color;
        aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &color);
        mat.diffuse = glm::vec3(color.r, color.g, color.b);
        aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &color);
        mat.specular = glm::vec3(color.r, color.g, color.b);
        aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &color);
        mat.ambient = glm::vec3(color.r, color.g, color.b);

		materials_map_[material_name] = mat;
	}
    renderables_.emplace_back(vertices, indices, &materials_map_[material_name]);

}

void MeshAssimp::LoadMaterialTextures(const aiMaterial* ai_mat, const aiTextureType type, std::vector<Texture>& textures)
{
    for (unsigned int i = 0; i < ai_mat->GetTextureCount(type); i++)
    {
        aiString ai_str;
        ai_mat->GetTexture(type, i, &ai_str);
        auto str = std::string(ai_str.data);
        if (texture_name_cache_.find(str) == texture_name_cache_.end())
        {
            texture_name_cache_.insert(str);
            textures.emplace_back(directory_ + "\\" + str);
        }
    }
}