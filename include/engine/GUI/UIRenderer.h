#pragma once
#include <string>
#include "engine/Component/Component.h"
#include "engine/Graphics/Mesh.h"
#include "engine/Graphics/Material.h"
#include "engine/Graphics/Shader.h"
#include "engine/Graphics/Texture.h"

class UIRenderer : public Component
{
public:
	UIRenderer(std::shared_ptr<GameObject> obj);
	vec4 color = vec4(0.5f, 0.5f, 0.5f, 1.0f);
	Shader* shader;

	void loadMesh(Mesh* mesh);
	void loadTexture(Texture* texture);
	void loadShader(Shader* shader);
	void render();

private:
	Mesh* _mesh = nullptr;
	std::vector<Texture*> _textures = std::vector<Texture*>();
	Shader* _shader = nullptr;
};
