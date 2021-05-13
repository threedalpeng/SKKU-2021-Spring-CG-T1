#include "ResourceManager.h"

Mesh* ResourceManager::getMesh(const std::string& meshName) {
	auto result = _meshList.find(meshName);
	return &(result->second);
}

Shader* ResourceManager::getShader(const std::string& shaderName) {
	auto result = _shaderList.find(shaderName);
	return &(result->second);
}

Material* ResourceManager::getMaterial(const std::string& materialName) {
	auto result = _materialList.find(materialName);
	return &(result->second);
}

Texture* ResourceManager::getTexture(const std::string& textureName) {
	auto result = _textureList.find(textureName);
	return &(result->second);
}

Mesh* ResourceManager::createMesh(const std::string& meshName, const std::string& vertexBinaryPath, const std::string& indexBinaryPath) {
	auto result = _meshList.emplace(meshName, Mesh(vertexBinaryPath, indexBinaryPath));
	if (result.second) {
		return &(result.first->second);
	}
	else {
		return nullptr;
	}
}

Shader* ResourceManager::createShader(const std::string& shaderName, const std::string& vertexShaderPath, const std::string& fragShaderPath) {
	auto result = _shaderList.emplace(shaderName, Shader(vertexShaderPath, fragShaderPath));
	if (result.second) {
		return &(result.first->second);
	}
	else {
		return nullptr;
	}
}

Material* ResourceManager::createMaterial(const std::string& materialName) {
	auto result = _materialList.emplace(materialName, Material());
	if (result.second) {
		return &(result.first->second);
	}
	else {
		return nullptr;
	}
}

Texture* ResourceManager::createTexture(const std::string& textureName, const std::string& texturePath) {
	auto result = _textureList.emplace(textureName, Texture(texturePath));
	if (result.second) {
		return &(result.first->second);
	}
	else {
		return nullptr;
	}
}