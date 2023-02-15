#include "MeshAssimp.h"

#include <functional>
#include <iostream>
#include <ostream>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/pbrmaterial.h>

namespace siho
{
	
}

void MeshAssimp::addFromFile(const std::string& path, std::map<std::string, siho::MaterialInstance*>& materials,
	bool override_material)
{
	Asset asset;
	asset.file = path;

    if (!setFromFile(asset, materials)) {
        return;
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

//void MeshAssimp::processGltfMaterial(const aiScene* scene, const aiMaterial* material, const std::string& material_name,
//	const std::string& dir_name, std::map<std::string, siho::MaterialInstance*>& out_materials) const
//{
//
//    aiString baseColorPath;
//    aiString AOPath;
//    aiString MRPath;
//    aiString normalPath;
//    aiString emissivePath;
//    aiTextureMapMode mapMode[3];
//    MaterialConfig matConfig;
//
//    material->Get(AI_MATKEY_TWOSIDED, matConfig.doubleSided);
//    material->Get(AI_MATKEY_GLTF_UNLIT, matConfig.unlit);
//
//    aiString alphaMode;
//    material->Get(AI_MATKEY_GLTF_ALPHAMODE, alphaMode);
//    if (strcmp(alphaMode.C_Str(), "BLEND") == 0) {
//        matConfig.alphaMode = AlphaMode::TRANSPARENT;
//    }
//    else if (strcmp(alphaMode.C_Str(), "MASK") == 0) {
//        matConfig.alphaMode = AlphaMode::MASKED;
//        float maskThreshold = 0.5;
//        material->Get(AI_MATKEY_GLTF_ALPHACUTOFF, maskThreshold);
//        matConfig.maskThreshold = maskThreshold;
//    }
//
//    material->Get(AI_MATKEY_GLTF_TEXTURE_TEXCOORD_BASE, AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_BASE_COLOR_TEXTURE,
//        matConfig.baseColorUV);
//    material->Get(_AI_MATKEY_GLTF_TEXTURE_TEXCOORD_BASE, AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_METALLICROUGHNESS_TEXTURE,
//        matConfig.metallicRoughnessUV);
//    material->Get(_AI_MATKEY_GLTF_TEXTURE_TEXCOORD_BASE, aiTextureType_LIGHTMAP, 0, matConfig.aoUV);
//    material->Get(_AI_MATKEY_GLTF_TEXTURE_TEXCOORD_BASE, aiTextureType_NORMALS, 0, matConfig.normalUV);
//    material->Get(_AI_MATKEY_GLTF_TEXTURE_TEXCOORD_BASE, aiTextureType_EMISSIVE, 0, matConfig.emissiveUV);
//
//    uint64_t configHash = hashMaterialConfig(matConfig);
//
//    if (mGltfMaterialCache.find(configHash) == mGltfMaterialCache.end()) {
//        mGltfMaterialCache[configHash] = createMaterialFromConfig(mEngine, matConfig);
//    }
//
//    outMaterials[materialName] = mGltfMaterialCache[configHash]->createInstance();
//
//    // TODO: is there a way to use the same material for multiple mask threshold values?
////    if (matConfig.alphaMode == masked) {
////        float maskThreshold = 0.5;
////        material->Get(AI_MATKEY_GLTF_ALPHACUTOFF, maskThreshold);
////        outMaterials[materialName]->setParameter("maskThreshold", maskThreshold);
////    }
//
//    // Load property values for gltf files
//    aiColor4D baseColorFactor;
//    aiColor3D emissiveFactor;
//    float metallicFactor = 1.0;
//    float roughnessFactor = 1.0;
//
//    // TODO: is occlusion strength available on Assimp now?
//
//    // Load texture images for gltf files
//    TextureSampler sampler(
//        TextureSampler::MinFilter::LINEAR_MIPMAP_LINEAR,
//        TextureSampler::MagFilter::LINEAR,
//        TextureSampler::WrapMode::REPEAT);
//
//    if (material->GetTexture(AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_BASE_COLOR_TEXTURE, &baseColorPath,
//        nullptr, nullptr, nullptr, nullptr, mapMode) == AI_SUCCESS) {
//        unsigned int minType = 0;
//        unsigned int magType = 0;
//        material->Get("$tex.mappingfiltermin", AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_BASE_COLOR_TEXTURE, minType);
//        material->Get("$tex.mappingfiltermag", AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_BASE_COLOR_TEXTURE, magType);
//
//        setTextureFromPath(scene, &mEngine, mTextures, baseColorPath,
//            materialName, dirName, mapMode, "baseColorMap", outMaterials, minType, magType);
//    }
//    else {
//        outMaterials[materialName]->setParameter("baseColorMap", mDefaultMap, sampler);
//    }
//
//    if (material->GetTexture(AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_METALLICROUGHNESS_TEXTURE, &MRPath,
//        nullptr, nullptr, nullptr, nullptr, mapMode) == AI_SUCCESS) {
//        unsigned int minType = 0;
//        unsigned int magType = 0;
//        material->Get("$tex.mappingfiltermin", AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_METALLICROUGHNESS_TEXTURE, minType);
//        material->Get("$tex.mappingfiltermag", AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_METALLICROUGHNESS_TEXTURE, magType);
//
//        setTextureFromPath(scene, &mEngine, mTextures, MRPath, materialName,
//            dirName, mapMode, "metallicRoughnessMap", outMaterials, minType, magType);
//    }
//    else {
//        outMaterials[materialName]->setParameter("metallicRoughnessMap", mDefaultMap, sampler);
//        outMaterials[materialName]->setParameter("metallicFactor", mDefaultMetallic);
//        outMaterials[materialName]->setParameter("roughnessFactor", mDefaultRoughness);
//    }
//
//    if (material->GetTexture(aiTextureType_LIGHTMAP, 0, &AOPath, nullptr,
//        nullptr, nullptr, nullptr, mapMode) == AI_SUCCESS) {
//        unsigned int minType = 0;
//        unsigned int magType = 0;
//        material->Get("$tex.mappingfiltermin", aiTextureType_LIGHTMAP, 0, minType);
//        material->Get("$tex.mappingfiltermag", aiTextureType_LIGHTMAP, 0, magType);
//        setTextureFromPath(scene, &mEngine, mTextures, AOPath, materialName,
//            dirName, mapMode, "aoMap", outMaterials, minType, magType);
//    }
//    else {
//        outMaterials[materialName]->setParameter("aoMap", mDefaultMap, sampler);
//    }
//
//    if (material->GetTexture(aiTextureType_NORMALS, 0, &normalPath, nullptr,
//        nullptr, nullptr, nullptr, mapMode) == AI_SUCCESS) {
//        unsigned int minType = 0;
//        unsigned int magType = 0;
//        material->Get("$tex.mappingfiltermin", aiTextureType_NORMALS, 0, minType);
//        material->Get("$tex.mappingfiltermag", aiTextureType_NORMALS, 0, magType);
//        setTextureFromPath(scene, &mEngine, mTextures, normalPath, materialName,
//            dirName, mapMode, "normalMap", outMaterials, minType, magType);
//    }
//    else {
//        outMaterials[materialName]->setParameter("normalMap", mDefaultNormalMap, sampler);
//    }
//
//    if (material->GetTexture(aiTextureType_EMISSIVE, 0, &emissivePath, nullptr,
//        nullptr, nullptr, nullptr, mapMode) == AI_SUCCESS) {
//        unsigned int minType = 0;
//        unsigned int magType = 0;
//        material->Get("$tex.mappingfiltermin", aiTextureType_EMISSIVE, 0, minType);
//        material->Get("$tex.mappingfiltermag", aiTextureType_EMISSIVE, 0, magType);
//        setTextureFromPath(scene, &mEngine, mTextures, emissivePath, materialName,
//            dirName, mapMode, "emissiveMap", outMaterials, minType, magType);
//    }
//    else {
//        outMaterials[materialName]->setParameter("emissiveMap", mDefaultMap, sampler);
//        outMaterials[materialName]->setParameter("emissiveFactor", mDefaultEmissive);
//    }
//
//    //If the gltf has texture factors, override the default factor values
//    if (material->Get(AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_METALLIC_FACTOR, metallicFactor) == AI_SUCCESS) {
//        outMaterials[materialName]->setParameter("metallicFactor", metallicFactor);
//    }
//
//    if (material->Get(AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_ROUGHNESS_FACTOR, roughnessFactor) == AI_SUCCESS) {
//        outMaterials[materialName]->setParameter("roughnessFactor", roughnessFactor);
//    }
//
//    if (material->Get(AI_MATKEY_COLOR_EMISSIVE, emissiveFactor) == AI_SUCCESS) {
//        sRGBColor emissiveFactorCast = *reinterpret_cast<sRGBColor*>(&emissiveFactor);
//        outMaterials[materialName]->setParameter("emissiveFactor", emissiveFactorCast);
//    }
//
//    if (material->Get(AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_BASE_COLOR_FACTOR, baseColorFactor) == AI_SUCCESS) {
//        sRGBColorA baseColorFactorCast = *reinterpret_cast<sRGBColorA*>(&baseColorFactor);
//        outMaterials[materialName]->setParameter("baseColorFactor", baseColorFactorCast);
//    }
//
//    aiBool isSpecularGlossiness = false;
//    if (material->Get(AI_MATKEY_GLTF_PBRSPECULARGLOSSINESS, isSpecularGlossiness) == AI_SUCCESS) {
//        if (isSpecularGlossiness) {
//            std::cout << "Warning: pbrSpecularGlossiness textures are not currently supported" << std::endl;
//        }
//    }
//}

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
                size_t indicesCount = num_faces * faces[0].mNumIndices;
                size_t indexBufferOffset = asset.indices.size();
                total_indices += indicesCount;

                for (size_t j = 0; j < num_faces; ++j) {
                    const aiFace& face = faces[j];
                    for (size_t k = 0; k < face.mNumIndices; ++k) {
                        asset.indices.push_back(static_cast<uint32_t>(face.mIndices[k] + indices_offset));
                    }
                }

                uint32_t materialId = mesh->mMaterialIndex;
                aiMaterial const* material = scene->mMaterials[materialId];

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

                asset.meshes.back().parts.push_back({
                        indexBufferOffset, indicesCount, material_name,
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
