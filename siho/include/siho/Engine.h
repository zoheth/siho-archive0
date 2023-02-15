#pragma once

#include <vector>

#include "BufferObject.h"
#include "Camera.h"
#include "Material.h"
#include "MaterialInstance.h"
#include "Renderer.h"
#include "Scene.h"
#include "Texture.h"
#include "VertexBuffer.h"
#include "View.h"

using std::vector;

namespace siho
{
	class Engine
	{
	public:
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

		void flush();
		bool execute();

	private:
		VertexBuffer* triangle_vb_ = nullptr;
		vector<BufferObject> buffer_objects_;
		vector<Renderer> renderers_;
		vector<View> views_;
		vector<VertexBuffer> vertex_buffers_;
		vector<Material> materials_;
		vector<Texture> textures_;
		


	};
}

