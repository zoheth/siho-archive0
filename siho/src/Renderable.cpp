#include <siho/Renderable.h>

void Renderable::SetMaterialUniforms(UniformObject& uniform_object)
{
	uniform_object.set(uniforms::kColorAmbient, material_->ambient);
	uniform_object.set(uniforms::kColorDiffuse, material_->diffuse);
	uniform_object.set(uniforms::kColorSpecular, material_->specular);
	uniform_object.set(uniforms::kShininess, material_->shininess);
	uniform_object.set(uniforms::kColorEmission, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	if(!material_->textures_diffuse.empty())
	{
		material_->textures_diffuse[0].Bind(texture_slot_++);
	}
	if(!material_->textures_specular.empty())
	{
		material_->textures_specular[0].Bind(texture_slot_++);
	}
	if(!material_->textures_normal.empty())
	{
		material_->textures_normal[0].Bind(texture_slot_++);
	}
}

void Renderable::setup()
{
	glGenVertexArrays(1, &vao_);
	glGenBuffers(1, &vbo_);
	glGenBuffers(1, &ebo_);

	glBindVertexArray(vao_);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex), vertices_.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(unsigned int), indices_.data(), GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), static_cast<void*>(0));
	glEnableVertexAttribArray(0);

	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Normal)));
	glEnableVertexAttribArray(1);

	// texCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, TexCoords)));
	glEnableVertexAttribArray(2);

	// vertex tangent
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Tangent)));

	// vertex bitangent
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Bitangent)));

	glBindVertexArray(0);
}

void Renderable::render(Shader& shader)
{
	UniformObject temp;
	SetMaterialUniforms(temp);
	temp.apply(shader);

	glBindVertexArray(vao_);
	glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
