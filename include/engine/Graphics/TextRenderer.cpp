#include "TextRenderer.h"
#include "engine/Screen.h"
#include "engine/Transform/Transform.h"
#include "engine/Object/GameObject.h"

Mesh* createPlaneMesh() {
	Mesh* mesh = new Mesh();

	mesh->vertex_list = {
		{ vec3(0,1,0), vec3(0,0,0), vec2(0.0f,0.0f) },
		{ vec3(0,0,0), vec3(0,0,0), vec2(0.0f,1.0f) },
		{ vec3(1,0,0), vec3(0,0,0), vec2(1.0f,1.0f) },
		{ vec3(0,1,0), vec3(0,0,0), vec2(0.0f,0.0f) },
		{ vec3(1,0,0), vec3(0,0,0), vec2(1.0f,1.0f) },
		{ vec3(1,1,0), vec3(0,0,0), vec2(1.0f,0.0f) }
	};

	glGenBuffers(1, &(mesh->vertex_buffer));
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * mesh->vertex_list.size(), &(mesh->vertex_list[0]), GL_STATIC_DRAW);

	mesh->vertex_array = cg_create_vertex_array(mesh->vertex_buffer); if (!(mesh->vertex_array)) { printf("%s(): VAO==nullptr\n", __func__); return nullptr; }
	return mesh;
}

TextRenderer::TextRenderer(std::shared_ptr<GameObject> obj) : Component(obj) {
	_mesh = createPlaneMesh();
}

void TextRenderer::setText(std::string text, vec4 color)
{
	_text = text;
	_color = color;
}

float TextRenderer::getTextWidth() {
	float width = 0.f;
	for (auto c : _text) {
		stbtt_char_t ch = _font->getCharacterInfo(c);
		width += ch.advance;
	}
	return width;
}

void TextRenderer::loadMesh(Mesh* mesh)
{
	_mesh = mesh;
}

void TextRenderer::loadMaterial(Material* material)
{
	_material = material;
}

void TextRenderer::loadFont(Font* font)
{
	_font = font;
}

void TextRenderer::loadShader(Shader* shader)
{
	_shader = shader;
}

void TextRenderer::render()
{
	glUseProgram(_shader->getProgram());

	float dpiScale = Screen::dpiScale();

	// use position.xy and scale.xy for x, y and scale
	Transform* transform = getComponent<Transform>();
	int tx = int(transform->position.x), ty = int(transform->position.y);
	float scaleX = transform->scale.x, scaleY = transform->scale.y;
	if (dpiScale > 1.0f) {
		tx = int(tx * dpiScale);
		ty = int(ty * dpiScale);
		scaleX *= dpiScale;
		scaleY *= dpiScale;
	}
	float x = float(tx), y = float(ty);

	if (_mesh && _mesh->getVertexArray())
		glBindVertexArray(_mesh->getVertexArray());

	glUniform4f(_shader->getUniformLocation("text_color"), _color.r, _color.g, _color.b, _color.a);
	glActiveTexture(GL_TEXTURE0);

	mat4 textOffsetMatrix = mat4(
		1 / (Screen::width() / 2.0f), 0.0f, 0.0f, -1.0f,		// view space conversion
		0.0f, 1 / (Screen::height() / 2.0f), 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	for (auto c : _text) {
		stbtt_char_t ch = _font->getCharacterInfo(c);

		mat4 textSizeMatrix = mat4(scaleX * float(ch.size.x), 0.0f, 0.0f, 0.0f,
			0.0f, scaleY * float(ch.size.y), 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
		mat4 textTranslateMatrix = mat4(1.0f, 0.0f, 0.0f, x + scaleX * float(ch.bearing.x),
			0.0f, 1.0f, 0.0f, -y + scaleY * float(-(ch.size.y - ch.bearing.y)),
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
		mat4 textMatrix = mat4();

		textMatrix = textOffsetMatrix * textTranslateMatrix * textSizeMatrix * textMatrix;

		glUniformMatrix4fv(_shader->getUniformLocation("text_matrix"), 1, GL_TRUE, textMatrix);
		glBindTexture(GL_TEXTURE_2D, ch.textureID);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// glDrawElements(GL_TRIANGLES, _mesh->index_list.size(), GL_UNSIGNED_INT, nullptr);

		x += ch.advance * scaleX;
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}