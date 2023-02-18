#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <siho/RenderableManager.h>
#include <siho/Engine.h>
#include <utils/Entity.h>

using namespace siho;
using namespace utils;

RenderableManager::Builder& RenderableManager::Builder::geometry(const std::vector<Vertex>& vertices,
                                                                 const std::vector<uint32_t>& indices, size_t offset, size_t count)
{
	glGenVertexArrays(1, &mVao);
	glGenBuffers(1, &mVbo);
	glGenBuffers(1, &mEbo);

	glBindVertexArray(mVao);

	glBindBuffer(GL_ARRAY_BUFFER, mVbo);
	glBufferData(GL_ARRAY_BUFFER, count * sizeof(Vertex), &vertices[offset], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo);
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
}
