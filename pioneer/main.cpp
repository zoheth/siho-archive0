#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/pbrmaterial.h>

int main()
{
    // 创建一个Importer对象
    Assimp::Importer importer;

    // 加载gltf模型文件
    const aiScene* scene = importer.ReadFile("C:\\3D Models\\lamp.gltf", aiProcess_Triangulate | aiProcess_FlipUVs);

    // 检查是否加载成功
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return -1;
    }

    // 遍历场景中的所有材质
    for (unsigned int i = 0; i < scene->mNumMaterials; i++)
    {
        // 获取第i个材质
        aiMaterial* material = scene->mMaterials[i];

        // 打印材质的名称
        aiString name;
        material->Get(AI_MATKEY_NAME, name);
        std::cout << "Material " << i << ": " << name.C_Str() << std::endl;

        // 打印材质的基本颜色
        aiColor4D baseColor;
        material->Get(AI_MATKEY_BASE_COLOR, baseColor);
        std::cout << "Base color: (" << baseColor.r << ", " << baseColor.g << ", " << baseColor.b << ", " << baseColor.a << ")" << std::endl;

        // 打印材质的金属度
        float metallic;
        material->Get(AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_METALLIC_FACTOR, metallic);
        std::cout << "Metallic: " << metallic << std::endl;

        // 打印材质的粗糙度
        float roughness;
        material->Get(AI_MATKEY_ROUGHNESS_FACTOR, roughness);
        std::cout << "Roughness: " << roughness << std::endl;

        // 打印材质的基本颜色纹理
        aiString baseColorTexture;
        material->GetTexture(AI_MATKEY_BASE_COLOR_TEXTURE, &baseColorTexture);
        std::cout << "Base color texture: " << baseColorTexture.C_Str() << std::endl;

        // 打印材质的金属度和粗糙度纹理
        aiString metallicRoughnessTexture;
        material->GetTexture(AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_METALLICROUGHNESS_TEXTURE, &metallicRoughnessTexture);
        std::cout << "Metallic roughness texture: " << metallicRoughnessTexture.C_Str() << std::endl;

        // 打印材质的法线纹理
        aiString normalTexture;
        material->GetTexture(aiTextureType_NORMALS, 0, &normalTexture);
        std::cout << "Normal texture: " << normalTexture.C_Str() << std::endl;

        // 打印材质的遮挡纹理
        aiString occlusionTexture;
        material->GetTexture(aiTextureType_AMBIENT_OCCLUSION, 0, &occlusionTexture);
        std::cout << "Occlusion texture: " << occlusionTexture.C_Str() << std::endl;

        // 打印材质的自发光纹理
        aiString emissiveTexture;
        material->GetTexture(aiTextureType_EMISSIVE, 0, &emissiveTexture);
        std::cout << "Emissive texture: " << emissiveTexture.C_Str() << std::endl;

        // 打印材质的透明度
        float opacity;
        material->Get(AI_MATKEY_OPACITY, opacity);
        std::cout << "Opacity: " << opacity << std::endl;

        // 打印材质的折射率
        float ior;
        material->Get(AI_MATKEY_REFRACTI, ior);
        std::cout << "Index of refraction: " << ior << std::endl;

        // 打印材质的双面性
        int twoSided;
        material->Get(AI_MATKEY_TWOSIDED, twoSided);
        std::cout << "Two sided: " << (twoSided ? "true" : "false") << std::endl;

        // 打印材质的混合模式
        aiBlendMode blendMode;
        material->Get(AI_MATKEY_BLEND_FUNC, blendMode);
        std::cout << "Blend mode: " << blendMode << std::endl;

        // 打印一个空行
        std::cout << std::endl;
    }

    return 0;
}
