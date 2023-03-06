#include <siho/Renderable.h>

#include <glad/glad.h>

void Renderable::SetMaterialUniforms(UniformObject& uniform_object)
{
	uniform_object.set(uniforms::kBaseColorFactor, material_->base_color_factor);
	uniform_object.set(uniforms::kEmissiveFactor, material_->emissive_factor);
	uniform_object.set(uniforms::kSpecularColorFactor, material_->specular_color_factor);

	uniform_object.set(uniforms::kMetallicFactor, material_->metallic_factor);
	uniform_object.set(uniforms::kRoughnessFactor, material_->roughness_factor);
	uniform_object.set(uniforms::kIor, material_->ior);


	unsigned int slot = texture_slot_;

	uniform_object.set(uniforms::kEmissiveTexture, 4);
	if (!material_->textures_emissive.empty())
	{
		material_->textures_emissive[0].Bind(4);
	}

	uniform_object.set(uniforms::kNormalTexture, 3);
	if (!material_->textures_normal.empty())
	{
		material_->textures_normal[0].Bind(3);
	}

	uniform_object.set(uniforms::kBaseColorTexture, 1);
	if(!material_->textures_base_color.empty())
	{
		material_->textures_base_color[0].Bind(1);
	}

	uniform_object.set(uniforms::kMetallicRoughnessTexture, 2);
	if(!material_->textures_roughness.empty())
	{
		material_->textures_roughness[0].Bind(2);
	}
}

void Renderable::Transform(glm::mat4 transform_matrix)
{
	model_matrix_=transform_matrix*model_matrix_;
}

void Renderable::setup()
{
	glGenVertexArrays(1, &vao_);
	glGenBuffers(1, &vbo_);
	glGenBuffers(1, &ebo_);

	glBindVertexArray(vao_);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex), vertices_.data(), GL_STATIC_DRAW);

	if(!indices_.empty())
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(unsigned int), indices_.data(), GL_STATIC_DRAW);
	}

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

void Renderable::render(Shader& shader, UniformObject uniform_object)
{
	SetMaterialUniforms(uniform_object);

	uniform_object.set(uniforms::kModel, model_matrix_);
	uniform_object.apply(shader);

	/*if(!material_->textures_emissive.empty())
	{
		glActiveTexture(GL_TEXTURE0+6);
		glBindTexture(GL_TEXTURE_2D, material_->textures_emissive[0].id());
		shader.setInt(uniforms::kEmissiveTexture, 6);
	}*/
	glBindVertexArray(vao_);
	if(indices_.empty())
		glDrawArrays(GL_TRIANGLES, 0, vertices_.size());
	else
		glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	for (int i = 0; i < 8; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	Texture::Unbind();
}
