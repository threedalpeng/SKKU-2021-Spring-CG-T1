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
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

#include "BulletDynamics/Featherstone/btMultiBodyDynamicsWorld.h"
#include "BulletDynamics/Featherstone/btMultiBodyConstraintSolver.h"
#include "BulletDynamics/Featherstone/btMultiBodyPoint2Point.h"
#include "BulletDynamics/Featherstone/btMultiBodyLinkCollider.h"

enum MyFilterModes
{
	FILTER_GROUPAMASKB_AND_GROUPBMASKA2 = 0,
	FILTER_GROUPAMASKB_OR_GROUPBMASKA2
};

struct MyOverlapFilterCallback2 : public btOverlapFilterCallback
{
	int m_filterMode;

	MyOverlapFilterCallback2()
		: m_filterMode(FILTER_GROUPAMASKB_AND_GROUPBMASKA2)
	{
	}

	virtual ~MyOverlapFilterCallback2()
	{
	}
	// return true when pairs need collision
	virtual bool needBroadphaseCollision(btBroadphaseProxy* proxy0, btBroadphaseProxy* proxy1) const
	{
		if (m_filterMode == FILTER_GROUPAMASKB_AND_GROUPBMASKA2)
		{
			bool collides = (proxy0->m_collisionFilterGroup & proxy1->m_collisionFilterMask) != 0;
			collides = collides && (proxy1->m_collisionFilterGroup & proxy0->m_collisionFilterMask);
			return collides;
		}

		if (m_filterMode == FILTER_GROUPAMASKB_OR_GROUPBMASKA2)
		{
			bool collides = (proxy0->m_collisionFilterGroup & proxy1->m_collisionFilterMask) != 0;
			collides = collides || (proxy1->m_collisionFilterGroup & proxy0->m_collisionFilterMask);
			return collides;
		}
		return false;
	}
};

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

		/* GameObject */
		GameObject* mainCamera = GameObject::create("Main Camera");
		GameObject* background = GameObject::create("Background Space");
		GameObject* lightPoint = GameObject::create("light point");

		GameObject* player = GameObject::create("player");
		GameObject* meteor = GameObject::create("meteor");

		//**********************************************
		// bullet init
		// initialize //
		// btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
		// btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
		// btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();
		// btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
		// GameManager::dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

		///collision configuration contains default setup for memory, collision setup
		btDefaultCollisionConfiguration* m_collisionConfiguration = new btDefaultCollisionConfiguration();
		//m_collisionConfiguration->setConvexConvexMultipointIterations();
		MyOverlapFilterCallback2* m_filterCallback = new MyOverlapFilterCallback2();
		///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
		btCollisionDispatcher* m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
		btOverlappingPairCache* m_pairCache = new btHashedOverlappingPairCache();
		m_pairCache->setOverlapFilterCallback(m_filterCallback);
		btBroadphaseInterface* m_broadphase = new btDbvtBroadphase(m_pairCache);  //btSimpleBroadphase();
		btMultiBodyConstraintSolver* m_solver = new btMultiBodyConstraintSolver;

		GameManager::dynamicsWorld = new btMultiBodyDynamicsWorld(m_dispatcher, m_broadphase, m_solver, m_collisionConfiguration);

		// dynamicsWorld->setGravity(btVector3(0, -10, 0));
		GameManager::dynamicsWorld->setGravity(btVector3(0, 0, 0));

		btAlignedObjectArray<btCollisionShape*> collisionShapes;

		GameObject* gui = GameObject::create("GUI");

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
			CustomRigidBody* body = new CustomRigidBody(rbInfo, objectTypes::BACKGROUND);

			//add the body to the dynamics world
			GameManager::dynamicsWorld->addRigidBody(body);
			body->gameObject = background;
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
			btCollisionShape* colShape = new btSphereShape(btScalar(0.6f));
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
			CustomRigidBody* body = new CustomRigidBody(rbInfo, objectTypes::PLAYER);

			GameManager::dynamicsWorld->addRigidBody(body);

			transform->body = body;
			body->setLinearVelocity(btVector3(0.0f, 0, 0));
			body->gameObject = player;
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
			btCollisionShape* colShape = new btSphereShape(btScalar(0.6f));
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
			CustomRigidBody* body = new CustomRigidBody(rbInfo, objectTypes::METEOR);

			GameManager::dynamicsWorld->addRigidBody(body);

			transform->body = body;
			body->setLinearVelocity(btVector3(-3.f, 0, 0));
			body->gameObject = meteor;
		}

		// GUI
		gui->addComponent<ScriptLoader>()->addScript(new Stage1GUIScript());
	}
};