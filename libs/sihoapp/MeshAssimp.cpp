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
            const size_t numFaces = mesh->mNumFaces;
            total_index_count += numFaces * faces[0].mNumIndices;
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

        asset.positions.reserve(asset.positions.size() + total_vertex_count);
        asset.tangents.reserve(asset.tangents.size() + total_vertex_count);
        asset.tex_coords0.reserve(asset.tex_coords0.size() + total_vertex_count);
        asset.tex_coords1.reserve(asset.tex_coords1.size() + total_vertex_count);
        asset.indices.reserve(asset.indices.size() + total_index_count);

        /*float2 minUV0 = float2(std::numeric_limits<float>::max());
        float2 maxUV0 = float2(std::numeric_limits<float>::lowest());
        getMinMaxUV(scene, node, minUV0, maxUV0, 0);
        float2 minUV1 = float2(std::numeric_limits<float>::max());
        float2 maxUV1 = float2(std::numeric_limits<float>::lowest());
        getMinMaxUV(scene, node, minUV1, maxUV1, 1);

        asset.snormUV0 = minUV0.x >= -1.0f && minUV0.x <= 1.0f && maxUV0.x >= -1.0f && maxUV0.x <= 1.0f &&
            minUV0.y >= -1.0f && minUV0.y <= 1.0f && maxUV0.y >= -1.0f && maxUV0.y <= 1.0f;

        asset.snormUV1 = minUV1.x >= -1.0f && minUV1.x <= 1.0f && maxUV1.x >= -1.0f && maxUV1.x <= 1.0f &&
            minUV1.y >= -1.0f && minUV1.y <= 1.0f && maxUV1.y >= -1.0f && maxUV1.y <= 1.0f;*/

        if (asset.snormUV0) {
            if (asset.snormUV1) {
                processNode<true, true>(asset, outMaterials,
                    scene, isGLTF, deep, mat_count, node, -1, depth);
            }
            else {
                processNode<true, false>(asset, outMaterials,
                    scene, isGLTF, deep, mat_count, node, -1, depth);
            }
        }
        else {
            if (asset.snormUV1) {
                processNode<false, true>(asset, outMaterials,
                    scene, isGLTF, deep, mat_count, node, -1, depth);
            }
            else {
                processNode<false, false>(asset, outMaterials,
                    scene, isGLTF, deep, mat_count, node, -1, depth);
            }
        }

        // compute the aabb and find bounding box of entire model
        for (auto& mesh : asset.meshes) {
            mesh.aabb = RenderableManager::computeAABB(
                asset.positions.data(),
                asset.indices.data() + mesh.offset,
                mesh.count);

            Box transformedAabb = computeTransformedAABB(
                asset.positions.data(),
                asset.indices.data() + mesh.offset,
                mesh.count,
                mesh.accTransform);

            float3 aabbMin = transformedAabb.getMin();
            float3 aabbMax = transformedAabb.getMax();

            if (!isinf(aabbMin.x) && !isinf(aabbMax.x)) {
                if (minBound.x > maxBound.x) {
                    minBound.x = aabbMin.x;
                    maxBound.x = aabbMax.x;
                }
                else {
                    minBound.x = fmin(minBound.x, aabbMin.x);
                    maxBound.x = fmax(maxBound.x, aabbMax.x);
                }
            }

            if (!isinf(aabbMin.y) && !isinf(aabbMax.y)) {
                if (minBound.y > maxBound.y) {
                    minBound.y = aabbMin.y;
                    maxBound.y = aabbMax.y;
                }
                else {
                    minBound.y = fmin(minBound.y, aabbMin.y);
                    maxBound.y = fmax(maxBound.y, aabbMax.y);
                }
            }

            if (!isinf(aabbMin.z) && !isinf(aabbMax.z)) {
                if (minBound.z > maxBound.z) {
                    minBound.z = aabbMin.z;
                    maxBound.z = aabbMax.z;
                }
                else {
                    minBound.z = fmin(minBound.z, aabbMin.z);
                    maxBound.z = fmax(maxBound.z, aabbMax.z);
                }
            }
        }

        return true;
    }
    return false;
}
