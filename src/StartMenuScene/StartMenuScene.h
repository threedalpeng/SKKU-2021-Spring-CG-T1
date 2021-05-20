#pragma once

// Standard Library
#include <iostream>

// Engine
#include "engine/Core.h"

// My Application
#include "Scripts/CameraScript.h"
#include "Scripts/BackgroundScript.h"
#include "Scripts/GameStartGUIScript.h"
#include "Scripts/PlayerAnimationScript.h"
#include "imgui/imgui.h"

#include "../Tool/MeshMaker.h"

class StartMenuScene : public Scene {
public:
	StartMenuScene() : Scene() {};

	void init() {
		/* Mesh */
		Mesh* cylinderMesh = MeshMaker::makeCylinderMesh();
		Mesh* boxMesh = createBoxMesh();
		Mesh* sphereMesh = MeshMaker::makeSphere();

		/* Texture */
		Texture* backgroundTexture = ResourceManager::getTexture("Milky_Way");
		Texture* headTexture = ResourceManager::getTexture("Player Head");

		/* Shader */
		// Shader* basicShader = new Shader("shaders/solar-system.vert", "shaders/solar-system.frag");

		/* GameObject */
		GameObject* background = GameObject::create("Background Space");

		GameObject* mainCamera = GameObject::create("Main Camera");

		// charater
		GameObject* player = GameObject::create("Player");
		GameObject* playerAxis = GameObject::create("Player Axis");
		GameObject* playerHead = GameObject::create("Player Head");
		GameObject* playerBody = GameObject::create("Player Body");
		GameObject* playerBack = GameObject::create("Player Back");
		GameObject* playerLeftArmAxis = GameObject::create("Player Left Arm Axis");
		GameObject* playerLeftArm = GameObject::create("Player Left Arm");
		GameObject* playerRightArmAxis = GameObject::create("Player Left Arm Axis");
		GameObject* playerRightArm = GameObject::create("Player Right Arm");
		GameObject* playerLeftLegAxis = GameObject::create("Player Left Leg Axis");
		GameObject* playerLeftLeg = GameObject::create("Player Left Leg");
		GameObject* playerRightLegAxis = GameObject::create("Player Right Leg Axis");
		GameObject* playerRightLeg = GameObject::create("Player Right Leg");

		GameObject* gameStartGUI = GameObject::create("Game Start GUI");

		/* Link Objects */
		addObject(background);

		addObject(player);
		/**/ player->addChildren(playerAxis);
		/**//**/ playerAxis->addChildren(playerHead);
		/**//**/ playerAxis->addChildren(playerBody);
		/**//**/ playerAxis->addChildren(playerBack);
		/**//**/ playerAxis->addChildren(playerLeftArmAxis);
		/**//**//**/ playerLeftArmAxis->addChildren(playerLeftArm);
		/**//**/ playerAxis->addChildren(playerRightArmAxis);
		/**//**//**/ playerRightArmAxis->addChildren(playerRightArm);
		/**//**/ playerAxis->addChildren(playerLeftLegAxis);
		/**//**//**/ playerLeftLegAxis->addChildren(playerLeftLeg);
		/**//**/ playerAxis->addChildren(playerRightLegAxis);
		/**//**//**/ playerRightLegAxis->addChildren(playerRightLeg);
		/**/ player->addChildren(mainCamera);

		addObject(gameStartGUI);

		/* Initialize Objects with Components */
		MeshRenderer* meshRenderer;
		Camera* camera;
		Transform* transform;
		SoundPlayer* soundPlayer;
		Material* material = new Material();

		// background
		{
			meshRenderer = background->addComponent<MeshRenderer>();
			meshRenderer->loadMesh(cylinderMesh);
			meshRenderer->loadTexture(backgroundTexture);
			meshRenderer->loadShader(GameManager::basicShader);
			meshRenderer->loadMaterial(material);
			meshRenderer->isShaded = false;
			meshRenderer->isColored = false;
			meshRenderer->hasTexture = true;

			transform = background->getComponent<Transform>();
			transform->scale = vec3(50, 100, 50);
			BackgroundScript* backgroundScript = new BackgroundScript();
			background->addComponent<ScriptLoader>()->addScript(backgroundScript);

			// soundPlayer = background->addComponent<SoundPlayer>();
			// soundPlayer->loadSoundFrom("sounds/hello.mp3");
			// soundPlayer->setType(SoundPlayer::Type::Background);
		}

		// main camera
		{
			camera = mainCamera->addComponent<Camera>();
			CameraScript* cameraScript = new CameraScript();
			mainCamera->addComponent<ScriptLoader>()->addScript(cameraScript);
			camera->addShader(GameManager::basicShader);
			meshRenderer->isShaded = false;
			meshRenderer->isColored = false;
			meshRenderer->hasTexture = false;

			camera->setThisMainCamera();

			// player
			PlayerAnimationScript* playerAnimationScript = new PlayerAnimationScript();
			playerAnimationScript->player = player->getComponent<Transform>();
			playerAnimationScript->leftArmAxis = playerLeftArmAxis->getComponent<Transform>();
			playerAnimationScript->rightArmAxis = playerRightArmAxis->getComponent<Transform>();
			playerAnimationScript->leftLegAxis = playerLeftLegAxis->getComponent<Transform>();
			playerAnimationScript->rightLegAxis = playerRightLegAxis->getComponent<Transform>();
			playerAxis->addComponent<ScriptLoader>()->addScript(playerAnimationScript);

			{// head
				transform = playerHead->getComponent<Transform>();
				transform->translate(1.5f, 0.f, 0.f);
				transform->scale = vec3(0.5f);
				meshRenderer = playerHead->addComponent<MeshRenderer>();
				meshRenderer->loadMesh(sphereMesh);
				meshRenderer->loadTexture(headTexture);
				meshRenderer->loadShader(GameManager::basicShader);
				meshRenderer->isShaded = false;
				//meshRenderer->isColored = true;
				//meshRenderer->color = vec4(1.f);
			}
			{// body
				transform = playerBody->getComponent<Transform>();
				transform->translate(0.f, 0.f, 0.f);
				transform->scale = vec3(1.0f, 0.5f, 0.25f);
				meshRenderer = playerBody->addComponent<MeshRenderer>();
				meshRenderer->loadMesh(boxMesh);
				meshRenderer->loadShader(GameManager::basicShader);
				meshRenderer->isShaded = false;
				meshRenderer->isColored = true;
				meshRenderer->color = vec4(1.f);

				transform = playerBack->getComponent<Transform>();
				transform->translate(0.f, 0.f, 0.5f);
				transform->scale = vec3(0.5f, 0.25f, 0.25f);
				meshRenderer = playerBack->addComponent<MeshRenderer>();
				meshRenderer->loadMesh(boxMesh);
				meshRenderer->loadShader(GameManager::basicShader);
				meshRenderer->isShaded = false;
				meshRenderer->isColored = true;
				meshRenderer->color = vec4(0.5f, 0.5f, 0.5f, 1.f);
			}
			{//arm
				transform = playerLeftArmAxis->getComponent<Transform>();
				transform->translate(0.75f, 0.75f, 0.f);

				transform = playerLeftArm->getComponent<Transform>();
				transform->translate(0.75f, 0.f, 0.f);
				transform->scale = vec3(1.f, 0.25f, 0.25f);
				meshRenderer = playerLeftArm->addComponent<MeshRenderer>();
				meshRenderer->loadMesh(boxMesh);
				meshRenderer->loadShader(GameManager::basicShader);
				meshRenderer->isShaded = false;
				meshRenderer->isColored = true;
				meshRenderer->color = vec4(1.f);

				transform = playerRightArmAxis->getComponent<Transform>();
				transform->translate(0.75f, -0.75f, 0.f);

				transform = playerRightArm->getComponent<Transform>();
				transform->translate(-0.75f, 0.f, 0.f);
				transform->scale = vec3(1.f, 0.25f, 0.25f);
				meshRenderer = playerRightArm->addComponent<MeshRenderer>();
				meshRenderer->loadMesh(boxMesh);
				meshRenderer->loadShader(GameManager::basicShader);
				meshRenderer->isShaded = false;
				meshRenderer->isColored = true;
				meshRenderer->color = vec4(1.f);
			}
			{// leg
				transform = playerLeftLegAxis->getComponent<Transform>();
				transform->translate(-1.25f, 0.5f, 0.f);

				transform = playerLeftLeg->getComponent<Transform>();
				transform->translate(-0.75f, 0.f, 0.f);
				transform->scale = vec3(1.0f, 0.25f, 0.25f);
				meshRenderer = playerLeftLeg->addComponent<MeshRenderer>();
				meshRenderer->loadMesh(boxMesh);
				meshRenderer->loadShader(GameManager::basicShader);
				meshRenderer->isShaded = false;
				meshRenderer->isColored = true;
				meshRenderer->color = vec4(1.f);

				transform = playerRightLegAxis->getComponent<Transform>();
				transform->translate(-1.25f, -0.5f, 0.f);

				transform = playerRightLeg->getComponent<Transform>();
				transform->translate(-0.75f, 0.f, 0.f);
				transform->scale = vec3(1.0f, 0.25f, 0.25f);
				meshRenderer = playerRightLeg->addComponent<MeshRenderer>();
				meshRenderer->loadMesh(boxMesh);
				meshRenderer->loadShader(GameManager::basicShader);
				meshRenderer->isShaded = false;
				meshRenderer->isColored = true;
				meshRenderer->color = vec4(1.f);
			}

			// gui
			GameStartGUIScript* guiScript = new GameStartGUIScript();
			gameStartGUI->addComponent<ScriptLoader>()->addScript(guiScript);
			soundPlayer = gameStartGUI->addComponent<SoundPlayer>();
			soundPlayer->loadSoundFrom("sounds/1 - Quite Space.mp3");
			soundPlayer->setType(SoundPlayer::Type::Background);
			soundPlayer->setVolume(0.5f);
		}
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