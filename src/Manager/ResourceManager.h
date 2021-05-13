#pragma once
#include <unordered_map>
#include "engine/Graphics/Mesh.h"
#include "engine/Graphics/Shader.h"
#include "engine/Graphics/Material.h"
#include "engine/Graphics/Texture.h"

class ResourceManager
{
public:
	void init() {
		/* If you don't wanna use for certain resource types, then don't set up in here */
	}

	Mesh* getMesh(const std::string& meshName);
	Shader* getShader(const std::string& shaderName);
	Material* getMaterial(const std::string& materialName);
	Texture* getTexture(const std::string& textureName);

	Mesh* createMesh(const std::string& meshName, const std::string& vertexBinaryPath, const std::string& indexBinaryPath);
	Shader* createShader(const std::string& shaderName, const std::string& vertexShaderPath, const std::string& fragShaderPath);
	Material* createMaterial(const std::string& materialName);
	Texture* createTexture(const std::string& textureName, const std::string& texturePath);
private:
	std::unordered_map<std::string, Mesh> _meshList;
	std::unordered_map<std::string, Shader> _shaderList;
	std::unordered_map<std::string, Material> _materialList;
	std::unordered_map<std::string, Texture> _textureList;
};
