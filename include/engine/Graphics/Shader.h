#pragma once
#include <string>
#include "cg/cgut.h"

class Shader
{
public:
	Shader();
	Shader(std::string vertexShaderPath, std::string indexShaderPath);
	~Shader();
	void clear();
	bool loadFrom(std::string vertexShaderPath, std::string indexShaderPath);
	GLuint getProgram();
	GLint getUniformLocation(std::string uniform_name);

private:
	GLuint program = 0;
};