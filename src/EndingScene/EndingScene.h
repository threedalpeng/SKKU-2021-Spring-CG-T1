#pragma once

// Standard Library
#include <iostream>

// Engine
#include "engine/Core.h"

// My Application
#include "Scripts/StartCameraScript.h"
#include "Scripts/StartBackgroundScript.h"
#include "Scripts/EndingGUI.h"
#include "Scripts/PlayerAnimationStartScript.h"
#include "imgui/imgui.h"

#include "../Tool/MeshMaker.h"

class StartMenuScene : public Scene {
public:
	StartMenuScene() : Scene() {};

	void init() {
		GameObject* gui = GameObject::create("Ending GUI");

		addObject(gui);

		/* Initialize Objects with Components */
		SoundPlayer* soundPlayer;

		GameStartGUIScript* guiScript = new GameStartGUIScript();
		gui->addComponent<ScriptLoader>()->addScript(guiScript);
		soundPlayer = gameStartGUI->addComponent<SoundPlayer>();
		soundPlayer->loadSoundFrom("sounds/1 - Quite Space.mp3");
		soundPlayer->setType(SoundPlayer::Type::Background);
		soundPlayer->setVolume(0.5f);
	}

private:

	Mesh* createCylinderMesh() {
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

	Mesh* createBoxMesh() {
		Mesh* mesh = new Mesh();

		uint nCircleVertex = 48;

		mesh->vertex_buffer = 0;
		mesh->index_buffer = 0;

		// Create vertex list
		mesh->vertex_list = {
			{ vec3(-1.f, -1.f, +1.f), vec3(-1.f, -1.f, +1.f), vec2(0.f, 0.f) },
			{ vec3(+1.f, -1.f, +1.f), vec3(+1.f, -1.f, +1.f), vec2(0.f, 0.f) },
			{ vec3(+1.f, -1.f, -1.f), vec3(+1.f, -1.f, -1.f), vec2(0.f, 0.f) },
			{ vec3(-1.f, -1.f, -1.f), vec3(-1.f, -1.f, -1.f), vec2(0.f, 0.f) },
			{ vec3(-1.f, +1.f, +1.f), vec3(-1.f, +1.f, +1.f), vec2(0.f, 0.f) },
			{ vec3(+1.f, +1.f, +1.f), vec3(+1.f, +1.f, +1.f), vec2(0.f, 0.f) },
			{ vec3(+1.f, +1.f, -1.f), vec3(+1.f, +1.f, -1.f), vec2(0.f, 0.f) },
			{ vec3(-1.f, +1.f, -1.f), vec3(-1.f, +1.f, -1.f), vec2(0.f, 0.f) },
		};

		// Create index list
		mesh->index_list = {
			0, 1, 4,
			1, 5, 4,
			1, 2, 5,
			2, 6, 5,
			0, 4, 3,
			4, 7, 3,
			7, 6, 2,
			3, 7, 2,
			7, 4, 5,
			6, 7, 5,
			0, 3, 1,
			1, 3, 2,
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