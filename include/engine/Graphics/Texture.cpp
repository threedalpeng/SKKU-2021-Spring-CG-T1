#include "Texture.h"

Texture::Texture(std::string texturePath, Type textureType) {
	loadFrom(texturePath);
	type = textureType;
}

void Texture::loadFrom(std::string texturePath)
{
	textureId = cg_create_texture(texturePath.c_str());
}