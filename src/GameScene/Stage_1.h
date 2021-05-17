#pragma once

// Standard Library
#include <iostream>

// Engine
#include "engine/Core.h"

// My Application
#include "Script/ObstacleScript.h"
#include "Script/PlayerScript.h"
#include "Script/Stage1GUIScript.h"

#include "../Tool/MeshMaker.h"
#include "../Custom/CustomRigidBody.h"

//*******************************************************************
// bullet3
#include "bullet/btBulletCollisionCommon.h"
#include "bullet/btBulletDynamicsCommon.h"

class Stage_1 : public Scene {
public:
	Stage_1() : Scene() {};

	void init() {
		/* Mesh */
		Mesh* cylinderMesh = MeshMaker::makeCylinderMesh();
		Mesh* sphereMesh = MeshMaker::makeSphere();

		/* Texture */
		Texture* backgroundTexture = new Texture("textures/Milky_Way.jpg");
		Texture* meteorTexture = new Texture("textures/venus.jpg");

		/* Shader */
		Shader* basicShader = new Shader("shaders/solar-system.vert", "shaders/solar-system.frag");

		/* GameObject */
		GameObject* mainCamera = GameObject::create("Main Camera");
		GameObject* background = GameObject::create("Background Space");
		GameObject* lightPoint = GameObject::create("light point");

		GameObject* player = GameObject::create("player");
		GameObject* meteor = GameObject::create("meteor");

		GameObject* gui = GameObject::create("GUI");

		/* Set Tags */
		background->setTag("Background");
		player->setTag("Player");
		meteor->setTag("Meteor");

		//*********************************************
		/* Link Objects */
		addObject(mainCamera);
		addObject(background);
		addObject(lightPoint);

		addObject(player);
		addObject(meteor);

		addObject(gui);

		/* Initialize Objects with Components */
		MeshRenderer* meshRenderer;
		Camera* camera;
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

		// background
		/*
		{
			Collider* collider = new Collider();
			collider->setCollisionShape(new btSphereShape(btScalar(50.0f)));
			collider->setLocalInertia(vec3(0.f, 0.f, 0.f));
			collider->setMass(1.f);
			RigidBody* rigidBody = player->addComponent<RigidBody>();
			rigidBody->loadCollider(collider);
			rigidBody->rigidBody()->setLinearVelocity(btVector3(0.f, 0.f, 0.f));
		}
		*/

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

		// player //
		{
			meshRenderer = player->addComponent<MeshRenderer>();
			meshRenderer->loadMesh(sphereMesh);
			meshRenderer->loadTexture(meteorTexture);
			meshRenderer->loadShader(basicShader);
			meshRenderer->loadMaterial(material);
			meshRenderer->isShaded = true;

			transform = player->getComponent<Transform>();
			transform->position = vec3(-3.0f, 0.0f, 0.0f);
			transform->scale = vec3(0.6f, 0.6f, 0.6f);
			PlayerScript* playerScript = new PlayerScript();
			player->addComponent<ScriptLoader>()->addScript(playerScript);

			Collider* collider = new Collider();
			collider->setCollisionShape(new btSphereShape(btScalar(0.6f)));
			collider->setLocalInertia(vec3(0.f, 0.f, 0.f));
			collider->setMass(1.f);
			RigidBody* rigidBody = player->addComponent<RigidBody>();
			rigidBody->loadCollider(collider);
			rigidBody->rigidBody()->setLinearVelocity(btVector3(0.f, 0.f, 0.f));
		}

		// meteor //
		{
			meshRenderer = meteor->addComponent<MeshRenderer>();
			meshRenderer->loadMesh(sphereMesh);
			meshRenderer->loadTexture(meteorTexture);
			meshRenderer->loadShader(basicShader);
			meshRenderer->loadMaterial(material);
			meshRenderer->isShaded = true;

			transform = meteor->getComponent<Transform>();
			transform->position = vec3(0.0f, 0.0f, 0.0f);
			transform->scale = vec3(0.6f, 0.6f, 0.6f);
			obstacleScript = new ObstacleScript(vec3(0.0f, 0, 0));
			meteor->addComponent<ScriptLoader>()->addScript(obstacleScript);

			Collider* collider = new Collider();
			collider->setCollisionShape(new btSphereShape(btScalar(0.6f)));
			collider->setLocalInertia(vec3(0.f, 0.f, 0.f));
			collider->setMass(1.f);
			RigidBody* rigidBody = meteor->addComponent<RigidBody>();
			rigidBody->loadCollider(collider);
			rigidBody->rigidBody()->setLinearVelocity(btVector3(-3.f, 0.f, 0.f));
		}

		// GUI
		gui->addComponent<ScriptLoader>()->addScript(new Stage1GUIScript());
	}
};