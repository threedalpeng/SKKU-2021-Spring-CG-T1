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
	bool isColored = false; 	// when it is true, fragment shader paint it by the color
	bool hasTexture = false; 	// when it is true, fragment shader paint it by the texture
	bool hasAlpha = false;
	vec4 color = vec4(0.5f, 0.5f, 0.5f, 1.0f);		// for fragment shader, color value
	Shader* shader;		// it will be used to determine the Shader for this Renderer

	void loadMesh(Mesh* mesh);
	void loadMaterial(Material* material);
	void loadTexture(Texture* texture);
	void loadShader(Shader* shader);
	void loadShaderDepth(Shader* shader);
	void render();
	void renderDepth();
private:
	Mesh* _mesh = nullptr;
	Material* _material = nullptr;
	std::vector<Texture*> _textures = std::vector<Texture*>();
	Shader* _shader = nullptr;
	Shader* _shaderDepth = nullptr;
};
