#pragma once
#include <string>

#include "engine/Component/Component.h"
#include "engine/Graphics/TextFont.h"
#include "engine/Graphics/Mesh.h"
#include "engine/Graphics/Material.h"
#include "engine/Graphics/Shader.h"

class TextRenderer : public Component
{
public:
	TextRenderer(std::shared_ptr<GameObject> obj);
	~TextRenderer();

	bool isShaded = true;

	void setText(std::string text, vec4 color);
	std::string text();
	vec4 color();

	vec2 getTextSize();
	void fitToRectangle(vec2 topRight, vec2 bottomLeft, bool fixRatio = true);

	void loadMesh(Mesh* mesh);
	void loadMaterial(Material* material);
	void loadFont(TextFont* font);
	void loadShader(Shader* shader);
	void render();

private:
	Mesh* _mesh = nullptr;
	Material* _material = nullptr;
	TextFont* _font = nullptr;
	Shader* _shader = nullptr;

	vec4 _color = vec4(0.2f, 0.8f, 0.2f, 1.0f);
	std::string _text = "";
};
