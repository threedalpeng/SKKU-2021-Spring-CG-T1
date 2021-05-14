#pragma once
#include <unordered_map>
#include "engine/Graphics/Mesh.h"
#include "engine/Graphics/Shader.h"
#include "engine/Graphics/Material.h"
#include "engine/Graphics/Texture.h"

class ResourceManager
{
public:
	static void init() {
		/* If you don't wanna use for certain resource types, then don't set up in here */
		// example
		createMesh("dragon", "mesh/dragon.index.bin", "mesh/dragon.vertex.bin");
		// then you can get it like: ResourceManager::getMesh("dragon");

		// If you have to set initial state of resource by your own,
		// you can initiate like:
		Material* material = createMaterial("shine");
		material->shininess = 1000.f;
	}

	static Mesh* getMesh(const std::string& meshName);
	static Shader* getShader(const std::string& shaderName);
	static Material* getMaterial(const std::string& materialName);
	static Texture* getTexture(const std::string& textureName);

	static Mesh* createMesh(const std::string& meshName, const std::string& vertexBinaryPath, const std::string& indexBinaryPath);
	static Shader* createShader(const std::string& shaderName, const std::string& vertexShaderPath, const std::string& fragShaderPath);
	static Material* createMaterial(const std::string& materialName);
	static Texture* createTexture(const std::string& textureName, const std::string& texturePath);
private:
	static std::unordered_map<std::string, Mesh> _meshList;
	static std::unordered_map<std::string, Shader> _shaderList;
	static std::unordered_map<std::string, Material> _materialList;
	static std::unordered_map<std::string, Texture> _textureList;
};

std::unordered_map<std::string, Mesh> ResourceManager::_meshList = std::unordered_map<std::string, Mesh>();
std::unordered_map<std::string, Shader> ResourceManager::_shaderList = std::unordered_map<std::string, Shader>();
std::unordered_map<std::string, Material> ResourceManager::_materialList = std::unordered_map<std::string, Material>();
std::unordered_map<std::string, Texture> ResourceManager::_textureList = std::unordered_map<std::string, Texture>();