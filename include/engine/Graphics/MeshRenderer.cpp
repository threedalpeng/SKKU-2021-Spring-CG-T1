#include "MeshRenderer.h"
#include "engine/Transform/Transform.h"
#include "engine/Object/GameObject.h"

MeshRenderer::MeshRenderer(std::shared_ptr<GameObject> obj) : Component(obj) {}

void MeshRenderer::loadMesh(Mesh* mesh)
{
	_mesh = mesh;
}

void MeshRenderer::loadMaterial(Material* material)
{
	_material = material;
}

void MeshRenderer::loadTexture(Texture* texture)
{
	_textures.push_back(texture);
}

void MeshRenderer::loadShader(Shader* shader)
{
	_shader = shader;
}

void MeshRenderer::loadShaderDepth(Shader* shader)
{
	_shaderDepth = shader;
}

void MeshRenderer::renderDepth()
{
	if (!_shaderDepth) return;

	glUseProgram(_shaderDepth->getProgram());

	if (_mesh && _mesh->getVertexArray())
		glBindVertexArray(_mesh->getVertexArray());

	Transform* transform = getComponent<Transform>();
	mat4 model_matrix = transform->getModelMatrix();
	glUniformMatrix4fv(_shaderDepth->getUniformLocation("model_matrix"), 1, GL_TRUE, model_matrix);

	glDrawElements(GL_TRIANGLES, _mesh->index_list.size(), GL_UNSIGNED_INT, nullptr);
}

void MeshRenderer::render()
{
	if (!_shader) return;

	glUseProgram(_shader->getProgram());

	if (_mesh && _mesh->getVertexArray())
		glBindVertexArray(_mesh->getVertexArray());

	Transform* transform = getComponent<Transform>();
	mat4 model_matrix = transform->getModelMatrix();
	glUniformMatrix4fv(_shader->getUniformLocation("model_matrix"), 1, GL_TRUE, model_matrix);

	glUniform1i(_shader->getUniformLocation("b_shaded"), isShaded);
	glUniform1i(_shader->getUniformLocation("b_colored"), isColored);
	glUniform1i(_shader->getUniformLocation("b_texture"), hasTexture);
	glUniform1i(_shader->getUniformLocation("b_alpha_tex"), hasAlpha);
	glUniform4fv(_shader->getUniformLocation("color"), 1, color);

	if (_material) {
		glUniform4fv(_shader->getUniformLocation("Ka"), 1, _material->ambient);
		glUniform4fv(_shader->getUniformLocation("Kd"), 1, _material->diffuse);
		glUniform4fv(_shader->getUniformLocation("Ks"), 1, _material->specular);
		glUniform1f(_shader->getUniformLocation("shininess"), _material->shininess);
	}

	// setup texture
	// yet Sample, planned to be like this...
	if (_textures.empty()) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glUniform1i(_shader->getUniformLocation("b_texture"), false);
		//glUniform1i(_shader->getUniformLocation("b_alpha"), false);
		glUniform1i(_shader->getUniformLocation("TEX0"), 0);
	}
	else {
		for (auto texture : _textures) {
			switch (texture->type) {
			case Texture::Type::Default:
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texture->textureId);
				//glUniform1i(_shader->getUniformLocation("b_alpha"), false);
				glUniform1i(_shader->getUniformLocation("TEX0"), 0);
				break;
			case Texture::Type::Alpha:
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, texture->textureId);
				glUniform1i(_shader->getUniformLocation("b_alpha"), true);
				glUniform1i(_shader->getUniformLocation("TEX1"), 1);
				break;
			}
		}
	}

	glDrawElements(GL_TRIANGLES, _mesh->index_list.size(), GL_UNSIGNED_INT, nullptr);
}