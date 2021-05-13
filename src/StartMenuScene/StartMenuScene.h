#pragma once

// Standard Library
#include <iostream>

// Engine
#include "engine/Core.h"

// My Application
#include "Scripts/CameraScript.h"
#include "Scripts/BackgroundScript.h"
#include "Scripts/GameStartButtonScript.h"

#include "../Tool/MeshMaker.h"

class StartMenuScene : public Scene {
public:
	StartMenuScene() : Scene() {};

	void init() {
		/* Font */
		Font* textFont = new Font();
		textFont->loadFrom("fonts/consola.ttf");

		/* Mesh */
		Mesh* cylinderMesh = createCylinderMesh();
		Mesh* boxMesh = MeshMaker::makeBoxMesh();

		/* Texture */
		Texture* backgroundTexture = new Texture("textures/Milky_Way.jpg");

		/* Shader */
		Shader* basicShader = new Shader("shaders/solar-system.vert", "shaders/solar-system.frag");
		Shader* textShader = new Shader("shaders/text.vert", "shaders/text.frag");

		/* GameObject */
		GameObject* background = GameObject::create("Background Space");

		GameObject* mainCamera = GameObject::create("Main Camera");

		GameObject* gameStartText = GameObject::create("Game Start Text");
		GameObject* gameStartButton = GameObject::create("GameStart Button");

		/* Link Objects */
		addObject(background);
		addObject(mainCamera);
		addObject(gameStartText);
		addObject(gameStartButton);

		/* Initialize Objects with Components */
		MeshRenderer* meshRenderer;
		Camera* camera;
		TextRenderer* textRenderer;
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

		// game start text
		textRenderer = gameStartText->addComponent<TextRenderer>();
		textRenderer->loadFont(textFont);
		textRenderer->loadShader(textShader);
		transform = gameStartText->getComponent<Transform>();
		transform->position = vec3(100.f, 200.f, 0.f);
		//transform->scale = vec3(10.f, 10.f, 0.f);
		textRenderer->setText("Hello, World!", vec4(0.2f, 0.8f, 0.2f, 1.0f));

		meshRenderer = gameStartButton->addComponent<MeshRenderer>();
		meshRenderer->loadMesh(boxMesh);
		meshRenderer->loadShader(basicShader);
		meshRenderer->isShaded = false;
		meshRenderer->isColored = true;
		meshRenderer->color = vec4(0.2f, 0.2f, 0.8f, 1.0f);
		transform = gameStartButton->getComponent<Transform>();
		transform->scale = vec3(3.0f, 1.0f, 0.1f);
		GameStartButtonScript* gameStartButtonScript = new GameStartButtonScript();
		gameStartButton->addComponent<ScriptLoader>()->addScript(gameStartButtonScript);
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