#pragma once
#include <string>
#include "engine/Component/Component.h"
#include "engine/Graphics/Mesh.h"
#include "engine/Graphics/Material.h"
#include "engine/Graphics/Shader.h"

class MeshRenderer : public Component
{
public:
	MeshRenderer(std::shared_ptr<GameObject> obj);

	bool isShaded = true;
	bool isColored = false; 	// when it is true, fragment shader paint it by the color
	bool hasTexture = false; 	// when it is true, fragment shader paint it by the texture
	vec4 color = vec4(0.5f, 0.5f, 0.5f, 1.0f);		// for fragment shader, color value
	Shader* shader;		// it will be used to determine the Shader for this Renderer

	void loadMesh(Mesh* mesh);
	void loadMaterial(Material* material);
	void loadTexture(std::string texturePath, int textureType = 0);
	void render(Shader& shader);
private:
	Mesh* _mesh = nullptr;
	Material* _material = nullptr;
	GLuint textures[3] = { 0, };
};
