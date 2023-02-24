#pragma once

#include <vector>
#include <unordered_map>

#include "BufferObject.h"
#include "Camera.h"
#include "RenderableManager.h"
#include "LightManager.h"
#include "CameraManager.h"
#include "Scene.h"
#include "Texture.h"
#include "VertexBuffer.h"
#include "View.h"

using std::vector;
using std::unordered_map;

namespace siho
{
	class Material;
	class MaterialInstance;
	class Renderer;

	class Engine
	{
	public:
		unordered_map<std::string, uint32_t> mResourcePaths;

		static Engine* create();
		static void destroy(Engine* engine);

		BufferObject* createBufferObject();
		VertexBuffer* createVertexBuffer();
		MaterialInstance* createMaterialInstance(const Material* material,
			const MaterialInstance* other, const char* name) noexcept;
		Material* createMaterial();
		Texture* createTexture();

		Renderer* createRenderer();

		Scene* createScene();
		View* createView();

		Camera* createCamera(utils::Entity entity);

		void createRenderable(const RenderableManager::Builder& builder, utils::Entity entity);

		void flush();
		bool execute();

	private:
		Engine();

		utils::EntityManager& mEntityManager;
		RenderableManager mRenderableManager;
		LightManager mLightManager;
		CameraManager mCameraManager;

		VertexBuffer* mTriangleVb = nullptr;
		vector<BufferObject> mBufferObjects;
		vector<Renderer> mRenderers;
		vector<View> mViews;
		vector<Scene> mScenes;
		vector<VertexBuffer> mVertexBuffers;
		vector<Material> mMaterials;
		vector<Texture> mTextures;

	};
}

