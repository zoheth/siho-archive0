#include <siho/RenderableManager.h>
#include <siho/Engine.h>
#include <utils/Entity.h>

#include <glad/glad.h>

using namespace siho;
using namespace utils;

void RenderableManager::create(const Builder& builder, utils::Entity entity)
{
	mRenderables[entity.getId()] = { builder.mVao, builder.mIndicesCount, builder.mMaterialInstance };
}

Renderable* RenderableManager::getRenderable(utils::Entity entity)
{
	const auto it = mRenderables.find(entity.getId());
	if(it != mRenderables.end())
	{
		return &it->second;
	}
	return nullptr;
}

RenderableManager::Builder& RenderableManager::Builder::geometry(const std::vector<Vertex>& vertices,
                                                                 const std::vector<uint32_t>& indices, size_t offset, size_t count)
{
	uint32_t vbo, ebo;
	mIndicesCount= count;
	glGenVertexArrays(1, &mVao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(mVao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, count * sizeof(Vertex), &vertices[offset], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), &indices[offset], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), static_cast<void*>(0));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, texCoords)));
	// vertex tangent
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, tangent)));
	// vertex bitangent
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, bitangent)));

	glBindVertexArray(0);
	return *this;
}

RenderableManager::Builder& RenderableManager::Builder::material(MaterialInstance* materialInstance) noexcept
{
	mMaterialInstance = materialInstance;
	return *this;
}

RenderableManager::Builder::Result RenderableManager::Builder::build(Engine& engine, Entity entity)
{
	engine.createRenderable(*this, entity);
	return SUCCESS;
}
