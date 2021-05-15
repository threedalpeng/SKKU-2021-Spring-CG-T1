#pragma once

// Standard Library
#include <iostream>

// Engine
#include "engine/Core.h"

// My Application
#include "Script/ObstacleScript.h"
#include "Script/PlayerScript.h"

#include "../Tool/MeshMaker.h"

//*******************************************************************
// bullet3
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

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
		
		GameObject* player = GameObject::create("player");
		GameObject* meteor = GameObject::create("meteor");
		
		//**********************************************
		// bullet init
		// initialize //
		btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
		///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
		btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
		///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
		btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();
		///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
		btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
		GameManager::dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

		// dynamicsWorld->setGravity(btVector3(0, -10, 0));
		GameManager::dynamicsWorld->setGravity(btVector3(0, 0, 0));

		btAlignedObjectArray<btCollisionShape*> collisionShapes;

		//*********************************************
		/* Link Objects */
		addObject(mainCamera);
		addObject(background);
		addObject(lightPoint);
		
		addObject(player);
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
		// background //
		{
		btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(50.), btScalar(50.), btScalar(50.)));

		collisionShapes.push_back(groundShape);

		btTransform groundTransform;
		groundTransform.setIdentity();
		groundTransform.setOrigin(btVector3(0, -56, 0));

		btScalar mass(0.);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			groundShape->calculateLocalInertia(mass, localInertia);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);

		//add the body to the dynamics world
		GameManager::dynamicsWorld->addRigidBody(body);
		}

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


			//create a dynamic rigidbody
			btCollisionShape* colShape = new btSphereShape(btScalar(1.6f));
			collisionShapes.push_back(colShape);

			/// Create Dynamic Objects
			btTransform startTransform;
			startTransform.setIdentity();

			btScalar mass(1.f);

			//rigidbody is dynamic if and only if mass is non zero, otherwise static
			bool isDynamic = (mass != 0.f);

			btVector3 localInertia(0, 0, 0);
			if (isDynamic)
				colShape->calculateLocalInertia(mass, localInertia);

			startTransform.setOrigin(btVector3(-3.0f, 0.0f, 0.0f));

			//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
			btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
			btRigidBody* body = new btRigidBody(rbInfo);

			GameManager::dynamicsWorld->addRigidBody(body);

			transform->body = body;
			body->setLinearVelocity(btVector3(0, 0, 0));
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
			obstacleScript = new ObstacleScript(vec3(-2.0f, 0, 0));
			meteor->addComponent<ScriptLoader>()->addScript(obstacleScript);


			//create a dynamic rigidbody
			btCollisionShape* colShape = new btSphereShape(btScalar(1.6f));
			collisionShapes.push_back(colShape);

			/// Create Dynamic Objects
			btTransform startTransform;
			startTransform.setIdentity();

			btScalar mass(1.f);

			//rigidbody is dynamic if and only if mass is non zero, otherwise static
			bool isDynamic = (mass != 0.f);

			btVector3 localInertia(0, 0, 0);
			if (isDynamic)
				colShape->calculateLocalInertia(mass, localInertia);

			startTransform.setOrigin(btVector3(0.0f, 0.0f, 0.0f));

			//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
			btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
			btRigidBody* body = new btRigidBody(rbInfo);
			
			GameManager::dynamicsWorld->addRigidBody(body);

			transform->body = body;
			body->setLinearVelocity(btVector3(-10.f, 0, 0));			
		}		
	}
};