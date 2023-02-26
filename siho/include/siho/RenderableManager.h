#pragma once
#include <unordered_map>
#include <glm/glm.hpp>

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
		uint32_t vbo{};
		uint32_t ebo{};
		MaterialInstance const* materialInstance = nullptr;
	};

	class RenderableManager
	{
	public:
		static RenderableManager& get();
		void create();
		Renderable* getRenderable(utils::Entity entity);
		class Builder
		{
		public:
			explicit Builder() noexcept;
			Builder& geometry(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, size_t offset, size_t count);
			Builder& material(MaterialInstance* materialInstance) noexcept;
			Builder& transform(glm::mat4 const& transform) noexcept;
			Builder& castShadows(bool enable) noexcept;
			enum Result { ERROR = -1, SUCCESS = 0 };
			Result build(Engine& engine, utils::Entity entity);

		private:
			uint32_t mVao{};
			uint32_t mVbo{};
			uint32_t mEbo{};
			MaterialInstance* mMaterialInstance = nullptr;
		};
	private:
		std::unordered_map<uint32_t, Renderable> mRenderables;

	};
	
}
