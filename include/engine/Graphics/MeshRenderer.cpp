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

void MeshRenderer::loadTexture(std::string texture_path, int type)
{
	if (type >= 3) return;
	textures[type] = cg_create_texture(texture_path.c_str());
}

void MeshRenderer::render(Shader& shader)
{
	if (_mesh && _mesh->getVertexArray())
		glBindVertexArray(_mesh->getVertexArray());

	Transform* transform = getComponent<Transform>();
	mat4 model_matrix = transform->getModelMatrix();
	glUniformMatrix4fv(shader.getUniformLocation("model_matrix"), 1, GL_TRUE, model_matrix);

	glUniform1i(shader.getUniformLocation("b_shaded"), isShaded);
	glUniform1i(shader.getUniformLocation("b_colored"), isColored);
	glUniform4fv(shader.getUniformLocation("color"), 1, color);

	if (_material) {
		glUniform4fv(shader.getUniformLocation("Ka"), 1, _material->ambient);
		glUniform4fv(shader.getUniformLocation("Kd"), 1, _material->diffuse);
		glUniform4fv(shader.getUniformLocation("Ks"), 1, _material->specular);
		glUniform1f(shader.getUniformLocation("shininess"), _material->shininess);
	}

	// setup texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glUniform1i(shader.getUniformLocation("TEX0"), 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glUniform1i(shader.getUniformLocation("b_alpha_tex"), textures[1]);
	glUniform1i(shader.getUniformLocation("TEX1"), 1);
	if (textures[2]) {
	}

	glDrawElements(GL_TRIANGLES, _mesh->index_list.size(), GL_UNSIGNED_INT, nullptr);
}