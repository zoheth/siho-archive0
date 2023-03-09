#include <siho/MeshAssimp.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>

#include <siho/Texture.h>

#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"

void MeshAssimp::LoadModel(std::string const& path)
{
	Assimp::Importer importer;
    importer.SetPropertyInteger(AI_CONFIG_PP_PTV_NORMALIZE, 1);
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
        aiMatrix4x4 ai_mat = node->mTransformation;
		const glm::mat4 mat = glm::make_mat4(&ai_mat.a1);
		ProcessMesh(mesh, scene, mat);
	}
	for (size_t i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

void MeshAssimp::ProcessMesh(aiMesh* mesh, const aiScene* scene, glm::mat4 transformation)
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
        ProcessMaterial(material, mat);
		materials_map_[material_name] = mat;
	}
    renderables_.emplace_back(vertices, indices, &materials_map_[material_name]);
    renderables_.back().Transform(glm::transpose(transformation));
}

void MeshAssimp::ProcessMaterial(const aiMaterial* material, Material& output_material)
{
    aiString name;
    material->Get(AI_MATKEY_NAME, name);
    output_material.name = std::string(name.C_Str());

    if (aiColor4D base_color_factor; material->Get(AI_MATKEY_BASE_COLOR, base_color_factor) == AI_SUCCESS) {
        output_material.base_color_factor = glm::vec4(base_color_factor.r, base_color_factor.g, base_color_factor.b, base_color_factor.a);
    }

    if (float metallic_factor; material->Get(AI_MATKEY_METALLIC_FACTOR, metallic_factor) == aiReturn_SUCCESS) {
        output_material.metallic_factor = metallic_factor;
    }

    if (float roughness_factor; material->Get(AI_MATKEY_ROUGHNESS_FACTOR, roughness_factor) == aiReturn_SUCCESS) {
        output_material.roughness_factor = roughness_factor;
    }


    if (float ior; material->Get(AI_MATKEY_REFRACTI, ior) == aiReturn_SUCCESS) {
        output_material.ior = ior;
    }

    if (aiColor4D emissive_factor; material->Get(AI_MATKEY_COLOR_EMISSIVE, emissive_factor) == AI_SUCCESS) {
        output_material.emissive_factor = glm::vec3(emissive_factor.r, emissive_factor.g, emissive_factor.b);
    }

    if (aiColor4D specular_color_factor; material->Get(AI_MATKEY_COLOR_SPECULAR, specular_color_factor) == AI_SUCCESS) {
        output_material.specular_color_factor = glm::vec3(specular_color_factor.r, specular_color_factor.g, specular_color_factor.b);
    }

    aiString filename;
    if(material->GetTexture(AI_MATKEY_BASE_COLOR_TEXTURE, &filename)== aiReturn_SUCCESS)
    {
        LoadMaterialTextures(filename, output_material.textures_base_color);
    }
    if(material->GetTexture(AI_MATKEY_ROUGHNESS_TEXTURE, &filename) == aiReturn_SUCCESS)
    {
	    LoadMaterialTextures(filename, output_material.textures_roughness);
    }
    if(material->GetTexture(aiTextureType_NORMALS, 0, &filename) == aiReturn_SUCCESS)
    {
	    LoadMaterialTextures(filename, output_material.textures_normal);
    }
    if(material->GetTexture(aiTextureType_EMISSIVE, 0, &filename) == aiReturn_SUCCESS)
    {
	    LoadMaterialTextures(filename, output_material.textures_emissive);
    }

}

void MeshAssimp::LoadMaterialTextures(const aiString ai_str, std::vector<Texture>& textures)
{
    auto str = std::string(ai_str.data);
    if (texture_name_cache_.find(str) == texture_name_cache_.end())
    {
        texture_name_cache_.insert(str);
        textures.emplace_back(directory_ + "\\" + str);
        
    }
}
