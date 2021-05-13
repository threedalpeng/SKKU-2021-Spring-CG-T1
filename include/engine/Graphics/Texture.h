#pragma once
#include <string>
#include "cg/cgut.h"

class Texture
{
public:
	enum class Type {
		Default,
		Alpha,
		Normal
	};

	Texture(std::string texturePath, Type textureType = Type::Default);
	void loadFrom(std::string TexturePath);

	Type type = Type::Default;
	GLuint textureId = 0;
};
