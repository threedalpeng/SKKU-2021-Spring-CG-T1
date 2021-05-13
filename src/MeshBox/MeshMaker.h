#pragma once

// Standard Library
#include <iostream>

// Engine
#include "engine/Core.h"

class MeshMaker
{
public:
    static Mesh* makeBoxMesh()
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
};


