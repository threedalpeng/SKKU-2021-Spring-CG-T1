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
    static Mesh* makeSphere();
    static Mesh* makeRing();
	static Mesh* makeCylinderMesh();
};

Mesh* MeshMaker::makeBoxMesh()
{
    Mesh* mesh = new Mesh();

    mesh->vertex_buffer = 0;
    mesh->index_buffer = 0;

    // Create vertex list
    mesh->vertex_list = {
        { vec3(0,1,0), vec3(0,0,0), vec2(0.0f,0.0f) },
        { vec3(0,0,0), vec3(0,0,0), vec2(0.0f,1.0f) },
        { vec3(1,0,0), vec3(0,0,0), vec2(1.0f,1.0f) },
        { vec3(0,1,0), vec3(0,0,0), vec2(0.0f,0.0f) },
        { vec3(1,0,0), vec3(0,0,0), vec2(1.0f,1.0f) },
        { vec3(1,1,0), vec3(0,0,0), vec2(1.0f,0.0f) } 
    }; 

    // Create index list
    mesh->index_list = {
        {0, 1, 2,
        3, 4, 5}
    };
    
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

    mesh->vertex_buffer = 0;
    mesh->index_buffer = 0;

    uint	NUM_LATITUDE = 36 * 1;
    uint	NUM_LONGTIDUE = 72 * 1;
    uint	MORE_LONGTIDUE = 72 * 1 + 1;
    uint	NUM_INDICES = 0;

    // Create vertex list
    mesh->vertex_list = {{ vec3(0.0f, 0.0f, 1.0f), vec3(0,0,1), vec2(0.5f, 1.0f) }};     // top
    for( uint k=0; k < NUM_LATITUDE; k++ )
	{
		float theta =  k * (PI * 1.0f / float(NUM_LATITUDE));
		for( uint j=0; j < NUM_LONGTIDUE; j++ )
		{
			float psi = j * (PI * 2.0f / float(NUM_LONGTIDUE));
			float x=sin(theta)*cos(psi), y=sin(theta)*sin(psi), z=cos(theta);
			float color_x = cos(psi/2), color_y = cos(theta);
			// vec2 tc = vec2(color_x, color_y)*0.5f + 0.5f;
			vec2 tc = vec2(psi / (PI * 2.0f), (PI * 1.0f - theta)/ (PI * 1.0f));
			mesh->vertex_list.push_back( { vec3(x,y,z), vec3(x,y,z), tc} );
		}		

		float x=sin(theta)*cos(0.0f), y=sin(theta)*sin(0.0f), z=cos(theta);
		vec2 tc = vec2(1.0f, (PI * 1.0f - theta)/ (PI * 1.0f));
		mesh->vertex_list.push_back( { vec3(x,y,z), vec3(x,y,z), tc} );
	}
	mesh->vertex_list.push_back( { vec3(0.0f, 0.0f, -1.0f), vec3(0,0,-1.0f), vec2(0.5f,0.0f) } ); // bottom


    // Create index list
    for( uint j=0; j < NUM_LONGTIDUE; j++ )
	{
		mesh->index_list.push_back( 0 );	// top
		mesh->index_list.push_back( 1 + (j + 1) );
		mesh->index_list.push_back( 1 + j );
	}

    for( uint k=0; k < NUM_LATITUDE - 1; k++ )
	{
		for( uint j=0; j < NUM_LONGTIDUE; j++ )
		{

			mesh->index_list.push_back(1 + k * MORE_LONGTIDUE + j );
			mesh->index_list.push_back(1 + k * MORE_LONGTIDUE + (j+1) );
			mesh->index_list.push_back(1 + (k+1) * MORE_LONGTIDUE + (j+1) );

			mesh->index_list.push_back(1 + (k+1) * MORE_LONGTIDUE + j );
			mesh->index_list.push_back(1 + k * MORE_LONGTIDUE + j );
			mesh->index_list.push_back(1 + (k+1) * MORE_LONGTIDUE + (j+1) );
		}
	}
	for( uint j=0; j < NUM_LONGTIDUE; j++ )
	{
		mesh->index_list.push_back( 1+ (NUM_LATITUDE - 1) * MORE_LONGTIDUE + j );
		mesh->index_list.push_back( 1+ (NUM_LATITUDE - 1) * MORE_LONGTIDUE + (j+1) );
		mesh->index_list.push_back( mesh->vertex_list.size() - 1 );	//bottom
	}

	NUM_INDICES = mesh->index_list.size();

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

	for( uint k=0; k < NUM_RING; k++ )	//inner
	{
		float theta =  k * (PI * 2.0f / float(NUM_RING));
		float x=cos(theta) * 0.5f, y=sin(theta)* 0.5f, z=0.0f;
		vec2 tc = vec2(0.0f , (float)((k+1)%2) );
		mesh->vertex_list.push_back( { vec3(x,y,z), vec3(x,y,z), tc} );
	}
	for( uint k=0; k < NUM_RING; k++ )	//outter
	{
		float theta =  k * (PI * 2.0f / float(NUM_RING));
		float x=cos(theta) , y=sin(theta), z=0.0f;
		vec2 tc = vec2(1.0f , (float)((k+1)%2) );
		mesh->vertex_list.push_back( { vec3(x,y,z), vec3(x,y,z), tc} );
	}

    for( uint k=0; k < NUM_RING; k++ )	//outter
	{
		mesh->index_list.push_back(k);
		mesh->index_list.push_back(NUM_RING + k);
		mesh->index_list.push_back((k+1) % NUM_RING);

		mesh->index_list.push_back(NUM_RING + k);
		mesh->index_list.push_back(NUM_RING + (k+1) % NUM_RING);
		mesh->index_list.push_back((k+1) % NUM_RING);
	}

	for( uint k=0; k < NUM_RING; k++ )	//outter
	{
		mesh->index_list.push_back(k);
		mesh->index_list.push_back((k+1) % NUM_RING);
		mesh->index_list.push_back(NUM_RING + k);
		

		mesh->index_list.push_back(NUM_RING + k);
		mesh->index_list.push_back((k+1) % NUM_RING);
		mesh->index_list.push_back(NUM_RING + (k+1) % NUM_RING);
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

Mesh* MeshMaker::makeCylinderMesh() {
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