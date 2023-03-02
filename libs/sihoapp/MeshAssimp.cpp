#include "MeshAssimp.h"

#include <functional>
#include <iostream>
#include <ostream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/pbrmaterial.h>

#include <siho/Material.h>
#include "utils/EntityManager.h"

using namespace siho;
using namespace utils;

namespace siho
{
    struct Vertex;
}


struct MaterialConfig {
    bool double_sided = false;
    bool unlit = false;
    bool has_vertex_colors = false;
    float mask_threshold = 0.5f;
    uint8_t base_color_uv = 0;
    uint8_t metallic_roughness_uv = 0;
    uint8_t emissive_uv = 0;
    uint8_t ao_uv = 0;
    uint8_t normal_uv = 0;

    uint8_t maxUVIndex() {
        return std::max({ base_color_uv, metallic_roughness_uv, emissive_uv, ao_uv, normal_uv });
    }
};

MeshAssimp::MeshAssimp(siho::Engine& engine)
	:mEngine(engine)
{
    mDefaultColorMaterial = new Material(mEngine);
}

void MeshAssimp::addFromFile(const std::string& path, std::map<std::string, siho::MaterialInstance*>& materials,
                             bool override_material)
{
	Asset asset;
	asset.file = path;

    if (!setFromFile(asset, materials)) {
        return;
    }

    for(auto& mesh:asset.meshes)
    {
	    const size_t start_index = mRenderables.size();
        mRenderables.resize(start_index + mesh.parts.size());
        EntityManager::get().create(mesh.parts.size(), mRenderables.data() + start_index);

        RenderableManager::Builder builder;
        size_t part_index = 0;
        for(auto& part: mesh.parts)
        {
            auto it = materials.find(part.material);

            builder.geometry(asset.vertices, asset.indices, part.offset, part.count);

        	builder.material(it != materials.end() ? it->second : nullptr);
            builder.build(mEngine, mRenderables[start_index + part_index]);
            ++part_index;
        }
    }



}

std::string get_extension(const std::string& file)
{
	const auto index = file.rfind('.');
    if (index != std::string::npos && index != 0) {
        return file.substr(index + 1);
    }
    else {
        return "";
    }
}

using Assimp::Importer;
bool MeshAssimp::setFromFile(Asset& asset, std::map<std::string, siho::MaterialInstance*>& out_materials)
{
	Importer importer;
    aiScene const* scene = importer.ReadFile(asset.file,
        // normals and tangents
        aiProcess_GenSmoothNormals |
        aiProcess_CalcTangentSpace |
        // UV Coordinates
        aiProcess_GenUVCoords |
        // topology optimization
        aiProcess_FindInstances |
        aiProcess_OptimizeMeshes |
        aiProcess_JoinIdenticalVertices |
        // misc optimization
        aiProcess_ImproveCacheLocality |
        aiProcess_SortByPType |
        // we only support triangles
        aiProcess_Triangulate);
	const size_t index = importer.GetImporterIndex(get_extension(asset.file).c_str());
    const aiImporterDesc* importer_desc = importer.GetImporterInfo(index);
    bool is_gltf = importer_desc &&
        (!strncmp("glTF Importer", importer_desc->mName, 13) ||
            !strncmp("glTF2 Importer", importer_desc->mName, 14));

    if (!scene) {
        std::cout << "No scene" << std::endl;
    }

    if (scene && !scene->mRootNode) {
        std::cout << "No root node" << std::endl;
    }
    const std::function<void(aiNode const* node, size_t& total_vertex_count, size_t& total_index_count)>
        count_vertices = [scene, &count_vertices]
        (aiNode const* node, size_t& total_vertex_count, size_t& total_index_count) {
        for (size_t i = 0; i < node->mNumMeshes; i++) {
            aiMesh const* mesh = scene->mMeshes[node->mMeshes[i]];
            total_vertex_count += mesh->mNumVertices;

            const aiFace* faces = mesh->mFaces;
            const size_t num_faces = mesh->mNumFaces;
            total_index_count += num_faces * faces[0].mNumIndices;
        }

        for (size_t i = 0; i < node->mNumChildren; i++) {
            count_vertices(node->mChildren[i], total_vertex_count, total_index_count);
        }
    };

    if (scene) {
        size_t deep = 0;
        size_t depth = 0;
        size_t mat_count = 0;

        aiNode const* node = scene->mRootNode;

        size_t total_vertex_count = 0;
        size_t total_index_count = 0;

        count_vertices(node, total_vertex_count, total_index_count);

        asset.vertices.reserve(asset.vertices.size() + total_vertex_count);
        asset.indices.reserve(asset.indices.size() + total_index_count);


        processNode(asset, out_materials,
                    scene, is_gltf, deep, mat_count, node, -1, depth);


        //// compute the aabb and find bounding box of entire model
        //for (auto& mesh : asset.meshes) {
        //    mesh.aabb = RenderableManager::computeAABB(
        //        asset.positions.data(),
        //        asset.indices.data() + mesh.offset,
        //        mesh.count);

        //    Box transformedAabb = computeTransformedAABB(
        //        asset.positions.data(),
        //        asset.indices.data() + mesh.offset,
        //        mesh.count,
        //        mesh.accTransform);

        //    glm::vec3 aabbMin = transformedAabb.getMin();
        //    glm::vec3 aabbMax = transformedAabb.getMax();

        //    if (!isinf(aabbMin.x) && !isinf(aabbMax.x)) {
        //        if (minBound.x > maxBound.x) {
        //            minBound.x = aabbMin.x;
        //            maxBound.x = aabbMax.x;
        //        }
        //        else {
        //            minBound.x = fmin(minBound.x, aabbMin.x);
        //            maxBound.x = fmax(maxBound.x, aabbMax.x);
        //        }
        //    }

        //    if (!isinf(aabbMin.y) && !isinf(aabbMax.y)) {
        //        if (minBound.y > maxBound.y) {
        //            minBound.y = aabbMin.y;
        //            maxBound.y = aabbMax.y;
        //        }
        //        else {
        //            minBound.y = fmin(minBound.y, aabbMin.y);
        //            maxBound.y = fmax(maxBound.y, aabbMax.y);
        //        }
        //    }

        //    if (!isinf(aabbMin.z) && !isinf(aabbMax.z)) {
        //        if (minBound.z > maxBound.z) {
        //            minBound.z = aabbMin.z;
        //            maxBound.z = aabbMax.z;
        //        }
        //        else {
        //            minBound.z = fmin(minBound.z, aabbMin.z);
        //            maxBound.z = fmax(maxBound.z, aabbMax.z);
        //        }
        //    }
        //}

        return true;
    }
    return false;
}

void setTextureFromPath(Engine* engine,
    const aiString& texture_file,
    const std::string& material_name, const std::string& texture_directory,
    const char* parameter_name,
    std::map<std::string, MaterialInstance*>& out_materials, const bool gamma = false
)
{
    const auto filename = texture_directory + '\\' + std::string(texture_file.C_Str());
    auto it = engine->mResourcePaths.find(filename);
    if (it != engine->mResourcePaths.end())
    {
        out_materials[material_name]->setParameter(parameter_name, it->second);
        return;
    }

    uint32_t textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum internal_format;
        GLenum data_format;
        if (nrComponents == 1)
            internal_format = data_format = GL_RED;
        else if (nrComponents == 3)
        {
            internal_format = gamma ? GL_SRGB : GL_RGB;
            data_format = GL_RGB;
        }
        else if (nrComponents == 4)
        {
            internal_format = gamma ? GL_SRGB_ALPHA : GL_RGBA;
            data_format = GL_RGBA;
        }

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, data_format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << filename << std::endl;
        stbi_image_free(data);
    }

    out_materials[material_name]->setParameter(parameter_name, textureID);
    engine->mResourcePaths.insert({ filename,textureID });
}

void MeshAssimp::processNode(Asset& asset, std::map<std::string, siho::MaterialInstance*>& out_materials,
                             const aiScene* scene, bool is_gltf, size_t deep, size_t mat_count, const aiNode* node, int parent_index, size_t& depth) const
{
    glm::mat4 const& current = glm::transpose(*reinterpret_cast<glm::mat4 const*>(&node->mTransformation));

    size_t total_indices = 0;
    asset.parents.push_back(parent_index);
    asset.meshes.push_back(Mesh{});
    asset.meshes.back().offset = asset.indices.size();
    asset.meshes.back().transform = current;

    glm::mat4 parentTransform = parent_index >= 0 ? asset.meshes[parent_index].acc_transform : glm::mat4();
    asset.meshes.back().acc_transform = parentTransform * current;

    for (size_t i = 0; i < node->mNumMeshes; i++) {
        aiMesh const* mesh = scene->mMeshes[node->mMeshes[i]];

        auto const* positions = reinterpret_cast<glm::vec3 const*>(mesh->mVertices);
        auto const* tangents = reinterpret_cast<glm::vec3 const*>(mesh->mTangents);
        auto const* bitangents = reinterpret_cast<glm::vec3 const*>(mesh->mBitangents);
        auto const* normals = reinterpret_cast<glm::vec3 const*>(mesh->mNormals);
        auto const* tex_coords0 = reinterpret_cast<glm::vec3 const*>(mesh->mTextureCoords[0]);
        auto const* tex_coords1 = reinterpret_cast<glm::vec3 const*>(mesh->mTextureCoords[1]);

        const size_t num_vertices = mesh->mNumVertices;

        if (num_vertices > 0) {
            const aiFace* faces = mesh->mFaces;
            const size_t num_faces = mesh->mNumFaces;

            if (num_faces > 0) {
                size_t indices_offset = asset.vertices.size();

                for (size_t j = 0; j < num_vertices; j++) {
                    Vertex vertex;
                    vertex.normal = normals[j];

                    // Assimp always returns 3D tex coords but we only support 2D tex coords.
                    glm::vec2 tex_coord0 = tex_coords0 ? glm::vec2{tex_coords0[j].x, tex_coords0[j].y} : glm::vec2{ 0.0 };
                    glm::vec2 tex_coord1 = tex_coords1 ? glm::vec2{ tex_coords1[j].x, tex_coords1[j].y } : glm::vec2{ 0.0 };
                    // If the tangent and bitangent don't exist, make arbitrary ones. This only
                    // occurs when the mesh is missing texture coordinates, because assimp
                    // computes tangents for us. (search up for aiProcess_CalcTangentSpace)
                    if (!tangents) {
                        vertex.bitangent = normalize(cross(vertex.normal, glm::vec3{ 1.0, 0.0, 0.0 }));
                        vertex.tangent = normalize(cross(vertex.normal, vertex.bitangent));
                    }
                    else {
                        vertex.tangent = tangents[j];
                        vertex.bitangent = bitangents[j];
                    }

                    asset.vertices.push_back(vertex);
                }

                // Populate the index buffer. All faces are triangles at this point because we
                // asked assimp to perform triangulation.
                size_t indices_count = num_faces * faces[0].mNumIndices;
                size_t index_buffer_offset = asset.indices.size();
                total_indices += indices_count;

                for (size_t j = 0; j < num_faces; ++j) {
                    const aiFace& face = faces[j];
                    for (size_t k = 0; k < face.mNumIndices; ++k) {
                        asset.indices.push_back(static_cast<uint32_t>(face.mIndices[k] + indices_offset));
                    }
                }

                uint32_t material_id = mesh->mMaterialIndex;
                aiMaterial const* material = scene->mMaterials[material_id];

                aiString name;
                std::string material_name;

                

                if (material->Get(AI_MATKEY_NAME, name) != AI_SUCCESS) {
                    if (is_gltf) {
                        while (out_materials.find("_mat_" + std::to_string(mat_count))
                            != out_materials.end()) {
                            mat_count++;
                        }
                        material_name = "_mat_" + std::to_string(mat_count);
                    }
                    else {
                        material_name = AI_DEFAULT_MATERIAL_NAME;
                    }
                }
                else {
                    material_name = name.C_Str();
                }

                /*if (is_gltf && out_materials.find(material_name) == out_materials.end()) {
                    std::string dir_name = asset.file.getParent();
                    processGltfMaterial(scene, material, material_name, dir_name, out_materials);
                }*/
                out_materials[material_name] = mDefaultColorMaterial->createInstance(material_name.c_str());

                aiString diffuse_path;
                if (material->GetTexture(aiTextureType_DIFFUSE, 0, &diffuse_path))
                {
                    setTextureFromPath(&mEngine, diffuse_path, material_name, "", "diffuse_map",out_materials);
                }

                aiColor3D color;
                glm::vec3 base_color{ 1.0f };
                if (material->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS) {
                    base_color = *reinterpret_cast<glm::vec3*>(&color);
                }

                float opacity;
                if (material->Get(AI_MATKEY_OPACITY, opacity) != AI_SUCCESS) {
                    opacity = 1.0f;
                }
                if (opacity <= 0.0f) opacity = 1.0f;

                float shininess;
                if (material->Get(AI_MATKEY_SHININESS, shininess) != AI_SUCCESS) {
                    shininess = 0.0f;
                }

                // convert shininess to roughness
                float roughness = sqrt(2.0f / (shininess + 2.0f));

                float metallic = 0.0f;
                float reflectance = 0.5f;
                if (material->Get(AI_MATKEY_COLOR_SPECULAR, color) == AI_SUCCESS) {
                    // if there's a non-grey specular color, assume a metallic surface
                    if (color.r != color.g && color.r != color.b) {
                        metallic = 1.0f;
                        base_color = *reinterpret_cast<glm::vec3*>(&color);
                    }
                    else {
                        if (base_color.r == 0.0f && base_color.g == 0.0f && base_color.b == 0.0f) {
                            metallic = 1.0f;
                            base_color = *reinterpret_cast<glm::vec3*>(&color);
                        }
                        else {
                            // TODO: the conversion formula is correct
                            // reflectance = sqrtf(color.r / 0.16f);
                        }
                    }
                }
                out_materials[material_name]->setParameter("baseColor", base_color);
                out_materials[material_name]->setParameter("metallic", metallic);
                out_materials[material_name]->setParameter("roughness", roughness);
                out_materials[material_name]->setParameter("reflectance", reflectance);

                asset.meshes.back().parts.push_back({
                        index_buffer_offset, indices_count, material_name,
                        base_color, opacity, metallic, roughness, reflectance
                    });
            }
        }
    }

    if (node->mNumMeshes > 0) {
        asset.meshes.back().count = total_indices;
    }

    if (node->mNumChildren) {
        parent_index = static_cast<int>(asset.meshes.size()) - 1;
        deep++;
        depth = std::max(deep, depth);
        for (size_t i = 0, c = node->mNumChildren; i < c; i++) {
            processNode(asset, out_materials, scene,is_gltf, deep, mat_count, node->mChildren[i], parent_index, depth);
        }
        deep--;
    }
}