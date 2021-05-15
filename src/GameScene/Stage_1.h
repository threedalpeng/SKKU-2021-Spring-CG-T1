#pragma once

// Standard Library
#include <iostream>

// Engine
#include "engine/Core.h"

// My Application
#include "Script/ObstacleScript.h"

#include "../Tool/MeshMaker.h"

class Stage_1 : public Scene {
public:
	Stage_1() : Scene() {};

	void init() {
		/* Font */
		/*
		Font* textFont = new Font();
		textFont->loadFrom("fonts/consola.ttf");
		*/

		/* Mesh */
		Mesh* cylinderMesh = MeshMaker::makeCylinderMesh();
		Mesh* sphereMesh = MeshMaker::makeSphere();

		/* Texture */
		Texture* backgroundTexture = new Texture("textures/Milky_Way.jpg");
		Texture* meteorTexture = new Texture("textures/venus.jpg");

		/* Shader */
		Shader* basicShader = new Shader("shaders/solar-system.vert", "shaders/solar-system.frag");
		Shader* textShader = new Shader("shaders/text.vert", "shaders/text.frag");

		/* GameObject */
		GameObject* mainCamera = GameObject::create("Main Camera");
		GameObject* background = GameObject::create("Background Space");
		GameObject* lightPoint = GameObject::create("light point");
		
		GameObject* meteor = GameObject::create("meteor");

		/* Link Objects */
		addObject(mainCamera);
		addObject(background);
		addObject(lightPoint);
		
		addObject(meteor);

		/* Initialize Objects with Components */
		MeshRenderer* meshRenderer;
		Camera* camera;
		//TextRenderer* textRenderer;
		Transform* transform;
		Light* light;
		Material* material = new Material();
		ObstacleScript* obstacleScript;

		// main camera
		camera = mainCamera->addComponent<Camera>();
		CameraScript* cameraScript = new CameraScript();
		mainCamera->addComponent<ScriptLoader>()->addScript(cameraScript);
		camera->addShader(basicShader);
		camera->setThisMainCamera();

		// background
		meshRenderer = background->addComponent<MeshRenderer>();
		meshRenderer->loadMesh(cylinderMesh);
		meshRenderer->loadTexture(backgroundTexture);
		meshRenderer->loadShader(basicShader);
		meshRenderer->loadMaterial(material);
		meshRenderer->isShaded = false;
		transform = background->getComponent<Transform>();
		transform->scale = vec3(50, 100, 50);
		BackgroundScript* backgroundScript = new BackgroundScript();
		background->addComponent<ScriptLoader>()->addScript(backgroundScript);

		// light point
		meshRenderer = lightPoint->addComponent<MeshRenderer>();
		meshRenderer->loadMesh(sphereMesh);
		meshRenderer->loadShader(basicShader);
		meshRenderer->loadMaterial(material);
		meshRenderer->isShaded = false;
		meshRenderer->isColored = true;

		transform = lightPoint->getComponent<Transform>();
		transform->position = vec3(0.0f, 0.0f, 200.0f);

		light = lightPoint->addComponent<Light>();
		light->setType(Light::Type::Point);
		light->loadShader(basicShader);

		obstacleScript = new ObstacleScript(vec3(0.0f, 0, 0));
		lightPoint->addComponent<ScriptLoader>()->addScript(obstacleScript);

		// meteor
		meshRenderer = meteor->addComponent<MeshRenderer>();
		meshRenderer->loadMesh(sphereMesh);
		meshRenderer->loadTexture(meteorTexture);
		meshRenderer->loadShader(basicShader);
		meshRenderer->loadMaterial(material);
		meshRenderer->isShaded = true;

		transform = meteor->getComponent<Transform>();
		transform->position = vec3(0.0f, 0.0f, 0.0f);
		transform->scale = vec3(0.6f, 0.6f, 0.6f);
		obstacleScript = new ObstacleScript(vec3(-2.0f, 0, 0));
		meteor->addComponent<ScriptLoader>()->addScript(obstacleScript);
	}
};