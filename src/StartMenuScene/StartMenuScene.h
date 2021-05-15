#pragma once

// Standard Library
#include <iostream>

// Engine
#include "engine/Core.h"

// My Application
#include "Scripts/CameraScript.h"
#include "Scripts/BackgroundScript.h"
#include "Scripts/GameStartGUIScript.h"
#include "imgui/imgui.h"

#include "../Tool/MeshMaker.h"

class StartMenuScene : public Scene {
public:
	StartMenuScene() : Scene() {};

	void init() {
		/* Mesh */
		Mesh* cylinderMesh = createCylinderMesh();
		Mesh* boxMesh = MeshMaker::makeBoxMesh();
		Mesh* planeMesh = create2DRectMesh();

		/* Texture */
		Texture* backgroundTexture = new Texture("textures/Milky_Way.jpg");

		/* Shader */
		Shader* basicShader = new Shader("shaders/solar-system.vert", "shaders/solar-system.frag");
		Shader* textShader = new Shader("shaders/text.vert", "shaders/text.frag");

		/* GameObject */
		GameObject* background = GameObject::create("Background Space");

		GameObject* mainCamera = GameObject::create("Main Camera");

		GameObject* gameStartGUI = GameObject::create("Game Start GUI");

		/* Link Objects */
		addObject(background);
		addObject(mainCamera);
		addObject(gameStartGUI);

		/* Initialize Objects with Components */
		MeshRenderer* meshRenderer;
		Camera* camera;
		Transform* transform;

		// background
		meshRenderer = background->addComponent<MeshRenderer>();
		meshRenderer->loadMesh(cylinderMesh);
		meshRenderer->loadTexture(backgroundTexture);
		meshRenderer->loadShader(basicShader);
		meshRenderer->isShaded = false;
		transform = background->getComponent<Transform>();
		transform->scale = vec3(50, 100, 50);
		BackgroundScript* backgroundScript = new BackgroundScript();
		background->addComponent<ScriptLoader>()->addScript(backgroundScript);

		// main camera
		camera = mainCamera->addComponent<Camera>();
		CameraScript* cameraScript = new CameraScript();
		mainCamera->addComponent<ScriptLoader>()->addScript(cameraScript);
		camera->addShader(basicShader);
		camera->setThisMainCamera();

		GameStartGUIScript* guiScript = new GameStartGUIScript();
		gameStartGUI->addComponent<ScriptLoader>()->addScript(guiScript);
	}

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

	Mesh* create2DRectMesh() {
		Mesh* mesh = new Mesh();

		mesh->vertex_list = {
			{ vec3(0,1,0), vec3(0,0,0), vec2(0.0f,0.0f) },
			{ vec3(0,0,0), vec3(0,0,0), vec2(0.0f,1.0f) },
			{ vec3(1,0,0), vec3(0,0,0), vec2(1.0f,1.0f) },
			{ vec3(0,1,0), vec3(0,0,0), vec2(0.0f,0.0f) },
			{ vec3(1,0,0), vec3(0,0,0), vec2(1.0f,1.0f) },
			{ vec3(1,1,0), vec3(0,0,0), vec2(1.0f,0.0f) }
		};

		mesh->index_list = {
			1, 2, 3, 1, 3, 4
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

	Mesh* createBoxMesh() {
		Mesh* mesh = new Mesh();

		mesh->vertex_buffer = 0;
		mesh->index_buffer = 0;

		mesh->vertex_list.push_back({ vec3(0.5f, 0.5f, 0.5f), vec3(0, 0, -1.f), vec2(1.0f) });
		mesh->vertex_list.push_back({ vec3(-0.5f, 0.5f, 0.5f), vec3(0, 0, -1.f), vec2(1.0f) });
		mesh->vertex_list.push_back({ vec3(-0.5f, 0.5f, -0.5f), vec3(0, 0, -1.f), vec2(1.0f) });
		mesh->vertex_list.push_back({ vec3(0.5f, 0.5f, -0.5f), vec3(0, 0, -1.f), vec2(1.0f) });
		mesh->vertex_list.push_back({ vec3(0.5f, -0.5f, 0.5f), vec3(0, 0, -1.f), vec2(1.0f) });
		mesh->vertex_list.push_back({ vec3(-0.5f, -0.5f, 0.5f), vec3(0, 0, -1.f), vec2(1.0f) });
		mesh->vertex_list.push_back({ vec3(-0.5f, -0.5f, -0.5f), vec3(0, 0, -1.f), vec2(1.0f) });
		mesh->vertex_list.push_back({ vec3(0.5f, -0.5f, -0.5f), vec3(0, 0, -1.f), vec2(1.0f) });

		mesh->index_list.push_back(1);
		mesh->index_list.push_back(2);
		mesh->index_list.push_back(4);

		mesh->index_list.push_back(1);
		mesh->index_list.push_back(4);
		mesh->index_list.push_back(5);

		mesh->index_list.push_back(1);
		mesh->index_list.push_back(5);
		mesh->index_list.push_back(2);

		mesh->index_list.push_back(6);
		mesh->index_list.push_back(7);
		mesh->index_list.push_back(2);

		mesh->index_list.push_back(6);
		mesh->index_list.push_back(5);
		mesh->index_list.push_back(7);

		mesh->index_list.push_back(6);
		mesh->index_list.push_back(2);
		mesh->index_list.push_back(5);

		mesh->index_list.push_back(8);
		mesh->index_list.push_back(5);
		mesh->index_list.push_back(4);

		mesh->index_list.push_back(8);
		mesh->index_list.push_back(4);
		mesh->index_list.push_back(7);

		mesh->index_list.push_back(8);
		mesh->index_list.push_back(7);
		mesh->index_list.push_back(5);

		mesh->index_list.push_back(3);
		mesh->index_list.push_back(4);
		mesh->index_list.push_back(2);

		mesh->index_list.push_back(3);
		mesh->index_list.push_back(2);
		mesh->index_list.push_back(7);

		mesh->index_list.push_back(3);
		mesh->index_list.push_back(7);
		mesh->index_list.push_back(4);

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