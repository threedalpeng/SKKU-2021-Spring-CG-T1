#pragma once
#include <string>
#include "engine/Component/Component.h"
#include "engine/Graphics/Mesh.h"
#include "engine/Graphics/Material.h"
#include "engine/Graphics/Shader.h"
#include "engine/Graphics/Texture.h"

class MeshRenderer : public Component
{
public:
	MeshRenderer(std::shared_ptr<GameObject> obj);

	bool isShaded = true;

	void loadMesh(Mesh* mesh);
	void loadMaterial(Material* material);
	void loadTexture(Texture* texture);
	void loadShader(Shader* shader);
	void render();
private:
	Mesh* _mesh = nullptr;
	Material* _material = nullptr;
	std::vector<Texture*> _textures = std::vector<Texture*>();
	Shader* _shader = nullptr;
};
