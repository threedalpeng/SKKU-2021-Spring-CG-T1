#pragma once

// Standard Library
#include <iostream>

// Engine
#include "engine/Core.h"
#include "cg/cgmath.h"

class MeshMaker
{
public:
	static Mesh* makeBoxMesh();
	static Mesh* make3DBoxMesh();
	static Mesh* makeSphere();
	static Mesh* makeRing();
	static Mesh* makeSkyCylinderMesh();
	static Mesh* makeCylinderMesh();
};

Mesh* MeshMaker::make3DBoxMesh()
{
	Mesh* mesh = new Mesh();

	mesh->vertex_buffer = 0;
	mesh->index_buffer = 0;

	// Create vertex list
	mesh->vertex_list = {
		{ vec3(1,1,1), vec3(1,1,1), vec2(0.0f,1.0f) },
		{ vec3(-1,1,1), vec3(-1,1,1), vec2(1.0f,1.0f) },
		{ vec3(-1,1,-1), vec3(-1,1,-1), vec2(1.0f,0.0f) },
		{ vec3(1,1,-1), vec3(1,1,-1), vec2(0.0f,0.0f) },

		{ vec3(-1,1,1), vec3(-1,1,1), vec2(0.0f,1.0f) },
		{ vec3(-1,-1,1), vec3(-1,-1,1), vec2(1.0f,1.0f) },
		{ vec3(-1,-1,-1), vec3(-1,-1,-1), vec2(1.0f,0.0f) },
		{ vec3(-1,1,-1), vec3(-1,1,-1), vec2(0.0f,0.0f) },

		{ vec3(-1,-1,1), vec3(-1,-1,1), vec2(0.0f,1.0f) },
		{ vec3(1,-1,1), vec3(1,-1,1), vec2(1.0f,1.0f) },
		{ vec3(1,-1,-1), vec3(1,-1,-1), vec2(0.0f,0.0f) },
		{ vec3(-1,-1,-1), vec3(-1,-1,-1), vec2(0.0f,0.0f) },

		{ vec3(1,-1,1), vec3(1,-1,1), vec2(0.0f,1.0f) },
		{ vec3(1,1,1), vec3(1,1,1), vec2(1.0f,1.0f) },
		{ vec3(1,1,-1), vec3(1,1,-1), vec2(1.0f,0.0f) },
		{ vec3(1,-1,-1), vec3(1,-1,-1), vec2(0.0f,0.0f) },

		{ vec3(1,1,1), vec3(1,1,1), vec2(0.0f,1.0f) },
		{ vec3(-1,1,1), vec3(-1,1,1), vec2(1.0f,1.0f) },
		{ vec3(-1,-1,1), vec3(-1,-1,1), vec2(1.0f,0.0f) },
		{ vec3(1,-1,1), vec3(1,-1,1), vec2(0.0f,0.0f) },

		{ vec3(1,1,-1), vec3(1,1,-1), vec2(0.0f,1.0f) },
		{ vec3(-1,1,-1), vec3(-1,1,-1), vec2(1.0f,1.0f) },
		{ vec3(-1,-1,-1), vec3(-1,-1,-1), vec2(1.0f,0.0f) },
		{ vec3(1,-1,-1), vec3(1,-1,-1), vec2(0.0f,0.0f) },
	};

	// Create index list
	for (int i = 0; i < 4; i++)
	{
		int k = i * 4;
		mesh->index_list.push_back(k);
		mesh->index_list.push_back(k + 3);
		mesh->index_list.push_back(k + 1);

		mesh->index_list.push_back(k + 2);
		mesh->index_list.push_back(k + 1);
		mesh->index_list.push_back(k + 3);
	}
	{
		int k = 4 * 4;
		mesh->index_list.push_back(k);
		mesh->index_list.push_back(k + 1);
		mesh->index_list.push_back(k + 3);

		mesh->index_list.push_back(k + 2);
		mesh->index_list.push_back(k + 3);
		mesh->index_list.push_back(k + 1);
	}
	{
		int k = 4 * 5;
		mesh->index_list.push_back(k);
		mesh->index_list.push_back(k + 3);
		mesh->index_list.push_back(k + 1);

		mesh->index_list.push_back(k + 2);
		mesh->index_list.push_back(k + 1);
		mesh->index_list.push_back(k + 3);
	}

	glGenBuffers(1, &(mesh->vertex_buffer));
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * mesh->vertex_list.size(), &(mesh->vertex_list[0]), GL_STATIC_DRAW);
	glGenBuffers(1, &(mesh->index_buffer));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh->index_list.size(), &(mesh->index_list[0]), GL_STATIC_DRAW);

	mesh->vertex_array = cg_create_vertex_array(mesh->vertex_buffer, mesh->index_buffer);
	return mesh;
}

Mesh* MeshMaker::makeSphere()
{
	Mesh* mesh = new Mesh();
	uint N = 60;

	mesh->vertex_buffer = 0;
	mesh->index_buffer = 0;

	float theta = PI / float(N);
	float phi = 2.f * PI / float(N);

	/* create vertex list */
	mesh->vertex_list.push_back({ vec3(0.f, 1.f, 0.f), vec3(0.f, 1.f, 0.f), vec2(1.0f) });
	for (uint i = 1; i <= N - 1; i++)
	{
		float st = sin(theta * i), ct = cos(theta * i);
		float vtc = float(N - i) / float(N);
		for (uint j = 0; j <= N; j++)
		{
			float sp = sin(phi * j), cp = cos(phi * j);
			mesh->vertex_list.push_back({ vec3(st * cp, ct, st * sp), vec3(st * cp, ct, st * sp), vec2(float(N - j) / float(N + 1), vtc) });
		}
	}
	mesh->vertex_list.push_back({ vec3(0.f, -1.f, 0.f), vec3(0.f, -1.f, 0.f), vec2(0.f) });

	/* create index list */
	for (uint i = 0; i < N; i++)
	{
		mesh->index_list.push_back(0);
		mesh->index_list.push_back(i + 2);
		mesh->index_list.push_back(i + 1);
	}
	for (uint j = 1; j < N - 1; j++)
	{
		for (uint i = 0; i < N; i++)
		{
			mesh->index_list.push_back((j - 1) * (N + 1) + i + 1);
			mesh->index_list.push_back((j - 1) * (N + 1) + i + 2);
			mesh->index_list.push_back(j * (N + 1) + i + 1);
			mesh->index_list.push_back(j * (N + 1) + i + 1);
			mesh->index_list.push_back((j - 1) * (N + 1) + i + 2);
			mesh->index_list.push_back(j * (N + 1) + i + 2);
		}
	}
	for (uint i = 0; i < N; i++)
	{
		mesh->index_list.push_back((N - 2) * (N + 1) + i + 1);
		mesh->index_list.push_back((N - 2) * (N + 1) + i + 2);
		mesh->index_list.push_back(N * N);
	}

	glGenBuffers(1, &(mesh->vertex_buffer));
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * mesh->vertex_list.size(), &(mesh->vertex_list[0]), GL_STATIC_DRAW);

	glGenBuffers(1, &(mesh->index_buffer));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh->index_list.size(), &(mesh->index_list[0]), GL_STATIC_DRAW);

	mesh->vertex_array = cg_create_vertex_array(mesh->vertex_buffer, mesh->index_buffer);
	return mesh;
}

Mesh* MeshMaker::makeRing()
{
	Mesh* mesh = new Mesh();

	mesh->vertex_buffer = 0;
	mesh->index_buffer = 0;

	uint	NUM_RING = 10 * 4;
	uint	NUM_INDICES_R = 0;

	for (uint k = 0; k < NUM_RING; k++)	//inner
	{
		float theta = k * (PI * 2.0f / float(NUM_RING));
		float x = cos(theta) * 0.5f, y = sin(theta) * 0.5f, z = 0.0f;
		vec2 tc = vec2(0.0f, (float)((k + 1) % 2));
		mesh->vertex_list.push_back({ vec3(x,y,z), vec3(x,y,z), tc });
	}
	for (uint k = 0; k < NUM_RING; k++)	//outter
	{
		float theta = k * (PI * 2.0f / float(NUM_RING));
		float x = cos(theta), y = sin(theta), z = 0.0f;
		vec2 tc = vec2(1.0f, (float)((k + 1) % 2));
		mesh->vertex_list.push_back({ vec3(x,y,z), vec3(x,y,z), tc });
	}

	for (uint k = 0; k < NUM_RING; k++)	//outter
	{
		mesh->index_list.push_back(k);
		mesh->index_list.push_back(NUM_RING + k);
		mesh->index_list.push_back((k + 1) % NUM_RING);

		mesh->index_list.push_back(NUM_RING + k);
		mesh->index_list.push_back(NUM_RING + (k + 1) % NUM_RING);
		mesh->index_list.push_back((k + 1) % NUM_RING);
	}

	for (uint k = 0; k < NUM_RING; k++)	//outter
	{
		mesh->index_list.push_back(k);
		mesh->index_list.push_back((k + 1) % NUM_RING);
		mesh->index_list.push_back(NUM_RING + k);

		mesh->index_list.push_back(NUM_RING + k);
		mesh->index_list.push_back((k + 1) % NUM_RING);
		mesh->index_list.push_back(NUM_RING + (k + 1) % NUM_RING);
	}

	NUM_INDICES_R = mesh->index_list.size();

	glGenBuffers(1, &(mesh->vertex_buffer));
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * mesh->vertex_list.size(), &(mesh->vertex_list[0]), GL_STATIC_DRAW);
	glGenBuffers(1, &(mesh->index_buffer));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh->index_list.size(), &(mesh->index_list[0]), GL_STATIC_DRAW);

	mesh->vertex_array = cg_create_vertex_array(mesh->vertex_buffer, mesh->index_buffer);
	return mesh;
}

Mesh* MeshMaker::makeSkyCylinderMesh() {
	Mesh* mesh = new Mesh();

	uint nCircleVertex = 48;

	mesh->vertex_buffer = 0;
	mesh->index_buffer = 0;

	// Create vertex list
	float theta = 2.f * PI / float(nCircleVertex);
	for (uint i = 0; i <= nCircleVertex; i++) {
		float st = sinf(theta * i), ct = cosf(theta * i);
		mesh->vertex_list.push_back({ vec3(ct, 1.f, st), vec3(-ct, 1.f, -st), vec2(i / float(nCircleVertex), 1.f) });
		mesh->vertex_list.push_back({ vec3(ct, -1.f, st), vec3(-ct, -1.f, -st), vec2(i / float(nCircleVertex), 0.f) });
	}

	// Create index list
	for (uint i = 0; i < nCircleVertex; i++) {
		uint s = i * 2;
		mesh->index_list.push_back(s);
		mesh->index_list.push_back(s + 1);
		mesh->index_list.push_back(s + 2);
		mesh->index_list.push_back(s + 1);
		mesh->index_list.push_back(s + 3);
		mesh->index_list.push_back(s + 2);
	}

	glGenBuffers(1, &(mesh->vertex_buffer));
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * mesh->vertex_list.size(), &(mesh->vertex_list[0]), GL_STATIC_DRAW);
	glGenBuffers(1, &(mesh->index_buffer));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh->index_list.size(), &(mesh->index_list[0]), GL_STATIC_DRAW);

	mesh->vertex_array = cg_create_vertex_array(mesh->vertex_buffer, mesh->index_buffer);
	return mesh;
}

Mesh* MeshMaker::makeCylinderMesh() {
	Mesh* mesh = new Mesh();

	uint nCircleVertex = 48;

	mesh->vertex_buffer = 0;
	mesh->index_buffer = 0;

	/* Create vertex list */
	float theta = 2.f * PI / float(nCircleVertex);

	// Side
	for (uint i = 0; i <= nCircleVertex; i++) {
		float st = sinf(theta * i), ct = cosf(theta * i);
		mesh->vertex_list.push_back({ vec3(ct, 1.f, st), vec3(+ct, 1.f, +st), vec2(i / float(nCircleVertex), 1.f) });
		mesh->vertex_list.push_back({ vec3(ct, -1.f, st), vec3(+ct, -1.f, +st), vec2(i / float(nCircleVertex), 0.f) });
	}

	// Two circles for up and bottom
	mesh->vertex_list.push_back({ vec3(0.f, 1.f, 0.f), vec3(0.f, +1.f, 0.f), vec2(0.5f) }); // origin
	for (uint k = 0; k <= nCircleVertex; k++)
	{
		float ct = cos(theta * k), st = sin(theta * k);
		mesh->vertex_list.push_back({ vec3(ct, 1.f, st), vec3(0.f, +1.f, 0.0f), vec2(ct, st) * 0.5f + 0.5f });
	}

	mesh->vertex_list.push_back({ vec3(0.f, -1.f, 0.f), vec3(0.f, -1.f, 0.f), vec2(0.5f) }); // origin
	for (uint k = 0; k <= nCircleVertex; k++)
	{
		float ct = cos(theta * k), st = sin(theta * k);
		mesh->vertex_list.push_back({ vec3(ct, -1.f, st), vec3(0.f, -1.f, 0.0f), vec2(ct, st) * 0.5f + 0.5f });
	}

	/* Create index list */
	for (uint i = 0; i < nCircleVertex; i++) {
		uint s = i * 2;
		mesh->index_list.push_back(s);
		mesh->index_list.push_back(s + 2);
		mesh->index_list.push_back(s + 1);
		mesh->index_list.push_back(s + 1);
		mesh->index_list.push_back(s + 2);
		mesh->index_list.push_back(s + 3);
	}

	uint originIdx = (nCircleVertex + 1) * 2;
	for (uint k = originIdx; k < originIdx + nCircleVertex; k++)
	{
		mesh->index_list.push_back(originIdx);	// the origin
		mesh->index_list.push_back(k + 2);
		mesh->index_list.push_back(k + 1);
	}

	originIdx += (nCircleVertex + 2);
	for (uint k = originIdx; k < originIdx + nCircleVertex; k++)
	{
		mesh->index_list.push_back(originIdx);	// the origin
		mesh->index_list.push_back(k + 1);
		mesh->index_list.push_back(k + 2);
	}

	glGenBuffers(1, &(mesh->vertex_buffer));
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * mesh->vertex_list.size(), &(mesh->vertex_list[0]), GL_STATIC_DRAW);
	glGenBuffers(1, &(mesh->index_buffer));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh->index_list.size(), &(mesh->index_list[0]), GL_STATIC_DRAW);

	mesh->vertex_array = cg_create_vertex_array(mesh->vertex_buffer, mesh->index_buffer);
	return mesh;
}

Mesh* MeshMaker::makeBoxMesh() {
	Mesh* mesh = new Mesh();

	mesh->vertex_buffer = 0;
	mesh->index_buffer = 0;

	// Create vertex list
	mesh->vertex_list = {
		// front
		{ vec3(-1.f, -1.f, +1.f), vec3(-0.1f, -0.1f, +1.f), vec2(0.f, 1.f) }, // 0
		{ vec3(+1.f, -1.f, +1.f), vec3(0.1f, -0.1f, +1.f), vec2(1.f, 1.f) }, // 1
		{ vec3(-1.f, +1.f, +1.f), vec3(-0.1f, 0.1f, +1.f), vec2(0.f, 1.f) }, // 4
		{ vec3(+1.f, +1.f, +1.f), vec3(0.1f, 0.1f, +1.f), vec2(1.f, 1.f) }, // 5

		// left
		{ vec3(+1.f, -1.f, +1.f), vec3(+1.f, -0.1f, 0.1f), vec2(1.f, 1.f) }, // 1
		{ vec3(+1.f, -1.f, -1.f), vec3(+1.f, -0.1f, -0.1f), vec2(1.f, 0.f) }, // 2
		{ vec3(+1.f, +1.f, +1.f), vec3(+1.f, 0.1f, 0.1f), vec2(1.f, 1.f) }, // 5
		{ vec3(+1.f, +1.f, -1.f), vec3(+1.f, 0.1f, -0.1f), vec2(1.f, 0.f) }, // 6

		// right
		{ vec3(-1.f, -1.f, +1.f), vec3(-1.f, -0.1f, 0.1f), vec2(0.f, 1.f) }, // 0
		{ vec3(-1.f, +1.f, +1.f), vec3(-1.f, 0.1f, 0.1f), vec2(0.f, 1.f) }, // 4
		{ vec3(-1.f, -1.f, -1.f), vec3(-1.f, -0.1f, -0.1f), vec2(0.f, 0.f) }, // 3
		{ vec3(-1.f, +1.f, -1.f), vec3(-1.f, 0.1f, -0.1f), vec2(0.f, 0.f) }, // 7

		// back
		{ vec3(-1.f, +1.f, -1.f), vec3(-0.1f, 0.1f, -1.f), vec2(0.f, 0.f) }, // 7
		{ vec3(+1.f, +1.f, -1.f), vec3(0.1f, 0.1f, -1.f), vec2(1.f, 0.f) }, // 6
		{ vec3(-1.f, -1.f, -1.f), vec3(-0.1f, -0.1f, -1.f), vec2(0.f, 0.f) }, // 3
		{ vec3(+1.f, -1.f, -1.f), vec3(0.1f, -0.1f, -1.f), vec2(1.f, 0.f) }, // 2

		// top
		{ vec3(-1.f, +1.f, -1.f), vec3(-0.1f, +1.f, -0.1f), vec2(0.f, 0.f) }, // 7
		{ vec3(-1.f, +1.f, +1.f), vec3(-0.1f, +1.f, 0.1f), vec2(0.f, 1.f) }, // 4
		{ vec3(+1.f, +1.f, -1.f), vec3(0.1f, +1.f, -0.1f), vec2(1.f, 0.f) }, // 6
		{ vec3(+1.f, +1.f, +1.f), vec3(0.1f, +1.f, 0.1f), vec2(1.f, 1.f) }, // 5

		// bottom
		{ vec3(-1.f, -1.f, +1.f), vec3(-0.1f, -1.f, 0.1f), vec2(0.f, 1.f) }, // 0
		{ vec3(-1.f, -1.f, -1.f), vec3(-0.1f, -1.f, -0.1f), vec2(0.f, 0.f) }, // 3
		{ vec3(+1.f, -1.f, +1.f), vec3(0.1f, -1.f, 0.1f), vec2(1.f, 1.f) }, // 1
		{ vec3(+1.f, -1.f, -1.f), vec3(0.1f, -1.f, -0.1f), vec2(1.f, 0.f) }, // 2
	};

	// Create index list
	for (uint i = 0; i < 6; i++) {
		uint k = i * 4;
		mesh->index_list.push_back(k);
		mesh->index_list.push_back(k + 1);
		mesh->index_list.push_back(k + 2);
		mesh->index_list.push_back(k + 1);
		mesh->index_list.push_back(k + 3);
		mesh->index_list.push_back(k + 2);
	}

	glGenBuffers(1, &(mesh->vertex_buffer));
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * mesh->vertex_list.size(), &(mesh->vertex_list[0]), GL_STATIC_DRAW);
	glGenBuffers(1, &(mesh->index_buffer));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh->index_list.size(), &(mesh->index_list[0]), GL_STATIC_DRAW);

	mesh->vertex_array = cg_create_vertex_array(mesh->vertex_buffer, mesh->index_buffer);
	return mesh;
}