#pragma once

#include <vector>
#include <unordered_map>

#include "BufferObject.h"
#include "Camera.h"
#include "Material.h"
#include "MaterialInstance.h"
#include "RenderableManager.h"
#include "Renderer.h"
#include "Scene.h"
#include "Texture.h"
#include "VertexBuffer.h"
#include "View.h"

using std::vector;
using std::unordered_map;

namespace siho
{
	class MaterialInstance;

	class Engine
	{
	public:
		unordered_map<std::string, uint32_t> mResourcePaths;

		static Engine* create();
		static void destroy(Engine* engine);

		BufferObject* createBufferObject();
		VertexBuffer* createVertexBuffer();
		MaterialInstance* createMaterialInstance();
		Material* createMaterial();
		Texture* createTexture();

		Renderer* createRenderer();

		Scene* createScene();
		View* createView();

		Camera* createCamera();

		void createRenderable(const RenderableManager::Builder& builder, utils::Entity entity);

		void flush();
		bool execute();

	private:
		VertexBuffer* mTriangleVb = nullptr;
		vector<BufferObject> mBufferObjects;
		vector<Renderer> mRenderers;
		vector<View> mViews;
		vector<VertexBuffer> mVertexBuffers;
		vector<Material> mMaterials;
		vector<Texture> mTextures;

	};
}

