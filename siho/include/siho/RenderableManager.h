#pragma once
#include <glm/glm.hpp>

#include <unordered_map>

namespace utils
{
	class Entity;
}

namespace siho
{
	class Engine;
	class VertexBuffer;
	class IndexBuffer;
	class MaterialInstance;

	struct Vertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texCoords;
		glm::vec3 tangent;
		glm::vec3 bitangent;
	};

	struct Renderable
	{
		uint32_t vao{};
		size_t indicesCount{};
		MaterialInstance* materialInstance = nullptr;
	};

	class RenderableManager
	{
	public:
		explicit RenderableManager(Engine& engine) :mEngine(engine) {}

		class Builder
		{
		public:
			Builder& geometry(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, size_t offset, size_t count);
			Builder& material(MaterialInstance* materialInstance) noexcept;
			Builder& transform(glm::mat4 const& transform) noexcept;
			Builder& castShadows(bool enable) noexcept;
			enum Result { ERROR = -1, SUCCESS = 0 };
			Result build(Engine& engine, utils::Entity entity);

		private:
			friend class RenderableManager;
			uint32_t mVao{};
			size_t mIndicesCount{};
			MaterialInstance* mMaterialInstance = nullptr;
		};
		void create(const Builder& builder, utils::Entity entity);
		Renderable* getRenderable(utils::Entity entity);
	private:
		Engine& mEngine;
		std::unordered_map<uint32_t, Renderable> mRenderables;

	};
	
}
