#include "Texture.h"
#include <iostream>
Texture::Texture(std::string texturePath, Type textureType) {
	loadFrom(texturePath);
	type = textureType;
}

void Texture::loadFrom(std::string texturePath)
{
	textureId = cg_create_texture(texturePath.c_str()); 
	if(!textureId) std::cout << "there is fail to load texture, that is " << texturePath << std::endl; 
}