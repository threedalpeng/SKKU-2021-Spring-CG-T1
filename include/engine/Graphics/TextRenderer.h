#pragma once
#include <string>
#include "engine/Component/Component.h"
#include "engine/Graphics/Font.h"
#include "engine/Graphics/Mesh.h"
#include "engine/Graphics/Material.h"
#include "engine/Graphics/Shader.h"

class TextRenderer : public Component
{
public:
	TextRenderer(std::shared_ptr<GameObject> obj);

	bool isShaded = true;

	void setText(std::string text, vec4 color);

	float getTextWidth();
	//float getTextHeight();

	void loadMesh(Mesh* mesh);
	void loadMaterial(Material* material);
	void loadFont(Font* font);
	void loadShader(Shader* shader);
	void render();

private:
	Mesh* _mesh = nullptr;
	Material* _material = nullptr;
	Font* _font = nullptr;
	Shader* _shader = nullptr;

	vec4 _color = vec4(0.2f, 0.8f, 0.2f, 1.0f);
	std::string _text = "";

	/*
	ivec2 position = ivec2();
	float scale = scale;
	*/
};
