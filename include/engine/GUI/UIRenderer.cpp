#include "UIRenderer.h"
#include "engine/Transform/Transform.h"
#include "engine/Object/GameObject.h"

UIRenderer::UIRenderer(std::shared_ptr<GameObject> obj) : Component(obj) {}

void UIRenderer::loadMesh(Mesh* mesh)
{
	_mesh = mesh;
}

void UIRenderer::loadTexture(Texture* texture)
{
	_textures.push_back(texture);
}

void UIRenderer::loadShader(Shader* shader)
{
	_shader = shader;
}

void UIRenderer::render()
{
	glUseProgram(_shader->getProgram());

	if (_mesh && _mesh->getVertexArray())
		glBindVertexArray(_mesh->getVertexArray());

	Transform* transform = getComponent<Transform>();
	mat4 model_matrix = transform->getModelMatrix();
	glUniformMatrix4fv(_shader->getUniformLocation("model_matrix"), 1, GL_TRUE, model_matrix);

	glUniform4fv(_shader->getUniformLocation("color"), 1, color);

	// setup texture
	// yet Sample, planned to be like this...
	for (auto texture : _textures) {
		switch (texture->type) {
		case Texture::Type::Default:
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture->textureId);
			glUniform1i(_shader->getUniformLocation("b_alpha"), false);
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

	glDrawElements(GL_TRIANGLES, _mesh->index_list.size(), GL_UNSIGNED_INT, nullptr);
}