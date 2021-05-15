#include "ResourceManager.h"

std::unordered_map<std::string, Mesh> ResourceManager::_meshList = std::unordered_map<std::string, Mesh>();
std::unordered_map<std::string, Shader> ResourceManager::_shaderList = std::unordered_map<std::string, Shader>();
std::unordered_map<std::string, Material> ResourceManager::_materialList = std::unordered_map<std::string, Material>();
std::unordered_map<std::string, Texture> ResourceManager::_textureList = std::unordered_map<std::string, Texture>();
std::unordered_map<std::string, ImFont*> ResourceManager::_fontList = std::unordered_map<std::string, ImFont*>();

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

ImFont* ResourceManager::getFont(const std::string& fontName) {
	auto result = _fontList.find(fontName);
	return (result->second);
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

void ResourceManager::addFont(const std::string& fontName, ImFont* font) {
	_fontList.insert(std::make_pair(fontName, font));
}