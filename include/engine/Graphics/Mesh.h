#pragma once
#include "cg/cgut.h"

class Mesh
{
public:
	Mesh();
	Mesh(std::string vertexBinaryPath, std::string indexBinaryPath);
	~Mesh();

	void loadFrom(std::string vertexBinaryPath, std::string indexBinaryPath);
	GLuint getVertexArray();

	std::vector<vertex>	vertex_list;
	std::vector<uint>	index_list;
	GLuint	vertex_buffer = 0;
	GLuint	index_buffer = 0;
	GLuint	vertex_array = 0;
};
