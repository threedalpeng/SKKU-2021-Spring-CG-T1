#pragma once

// Standard Library
#include <iostream>

// Engine
#include "engine/Core.h"

// My Application
#include "Script/ObstacleScript.h"
#include "Script/Stage2PlayerScript.h"
#include "Script/PlayerAnimationScript.h"
#include "Script/GameCameraScript.h"
#include "Script/GameBackgroundScript.h"
#include "Script/Stage2GUIScript.h"
#include "Script/EmptyBoxScript.h"
#include "Script/SavePointScript.h"

#include "../Tool/MeshMaker.h"
#include "../Tool/ParticleMaker.h"
#include "../Tool/MeteorMaker.h"
#include "../Custom/CustomRigidBody.h"

//*******************************************************************
// bullet3
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#include "PhysicsUtil.h"

class Stage_2 : public Scene {
public:
	Stage_2() : Scene() {};
	~Stage_2() {
		EventManager<MeteorMoveEvent>::removeListener(meteorMoveEventId);
	}

	Texture* wallTexture = nullptr;
	Texture* radioactiveTexture = nullptr;
	Texture* meteorTexture = nullptr;

	btAlignedObjectArray<btCollisionShape*> collisionShapes;
	uint meteorMoveEventId;

	void init() {
		/* Mesh */
		Mesh* skyCylinderMesh = ResourceManager::getMesh("Sky Cylinder");
		Mesh* sphereMesh = ResourceManager::getMesh("Sphere");
		Mesh* boxMesh = ResourceManager::getMesh("Box");

		/* Texture */
		Texture* backgroundTexture = ResourceManager::getTexture("Milky_Way");
		meteorTexture = ResourceManager::getTexture("venus");
		Texture* fireTexture = ResourceManager::getTexture("fire");
		Texture* fireParticleTexture = ResourceManager::getTexture("fireParticle");
		Texture* whiteTexture = ResourceManager::getTexture("white");
		Texture* headTexture = ResourceManager::getTexture("Player Head");
		wallTexture = ResourceManager::getTexture("moon");
		radioactiveTexture = ResourceManager::getTexture("radioactive");

		/* Material */
		Material* material = ResourceManager::getMaterial("Basic");

		/* Shader */
		// Shader* basicShader = ResourceManager::getShader("basicShader");	 	// !!! bug

		/* GameObject */
		GameObject* mainCamera = GameObject::create("Main Camera");

		GameObject* background = GameObject::create("Background Space");
		GameObject* lightPoint = GameObject::create("light point");

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

		GameObject* backBox = GameObject::create("back box");
		GameObject* savePoint_1 = GameObject::create("save point 1");

		GameObject* meteor1 = GameObject::create("meteor1");
		GameObject* meteor2 = GameObject::create("meteor2");
		GameObject* meteor3 = GameObject::create("meteor3");

		GameObject* gui = GameObject::create("GUI");

		//**********************************************
		// bullet init
		btDefaultCollisionConfiguration* m_collisionConfiguration = new btDefaultCollisionConfiguration();
		MyOverlapFilterCallback2* m_filterCallback = new MyOverlapFilterCallback2();
		btCollisionDispatcher* m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
		btOverlappingPairCache* m_pairCache = new btHashedOverlappingPairCache();
		m_pairCache->setOverlapFilterCallback(m_filterCallback);
		btBroadphaseInterface* m_broadphase = new btDbvtBroadphase(m_pairCache);  //btSimpleBroadphase();
		btMultiBodyConstraintSolver* m_solver = new btMultiBodyConstraintSolver;

		GameManager::dynamicsWorld = new btMultiBodyDynamicsWorld(m_dispatcher, m_broadphase, m_solver, m_collisionConfiguration);

		GameManager::dynamicsWorld->setGravity(btVector3(0, 0, 0));

		//*********************************************
		/* Link Objects */

		//addObject(lightPoint);

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
		/**/ player->addChildren(background);
		/**/ player->addChildren(mainCamera);
		/**/ player->addChildren(lightPoint);
		addObject(backBox);

		addObject(meteor1);
		addObject(meteor2);
		addObject(meteor3);
		addObject(savePoint_1);

		addObject(gui);

		/* Initialize Objects with Components */
		MeshRenderer* meshRenderer;
		Camera* camera;
		//TextRenderer* textRenderer;
		Transform* transform;
		Light* light;
		SoundPlayer* soundPlayer;

		// main camera
		{
			camera = mainCamera->addComponent<Camera>();
			GameCameraScript* cameraScript = new GameCameraScript();
			cameraScript->distance = 40.f;
			mainCamera->addComponent<ScriptLoader>()->addScript(cameraScript);
			camera->addShader(GameManager::basicShader);
			camera->addShader(GameManager::depthShader);

			camera->setThisMainCamera();
		}

		// background //
		{
			meshRenderer = background->addComponent<MeshRenderer>();
			meshRenderer->loadMesh(skyCylinderMesh);
			meshRenderer->loadTexture(backgroundTexture);
			meshRenderer->loadShader(GameManager::basicShader);
			// meshRenderer->loadShaderDepth(GameManager::depthShader);
			meshRenderer->loadMaterial(material);
			meshRenderer->isShaded = false;
			meshRenderer->isColored = false;
			meshRenderer->hasTexture = true;
			meshRenderer->hasAlpha = false;

			transform = background->getComponent<Transform>();
			transform->scale = vec3(100, 100, 100);
			GameBackgroundScript* backgroundScript = new GameBackgroundScript();
			background->addComponent<ScriptLoader>()->addScript(backgroundScript);
		}

		// light point && depth camera
		{
			meshRenderer = lightPoint->addComponent<MeshRenderer>();
			meshRenderer->loadMesh(sphereMesh);
			meshRenderer->loadShader(GameManager::basicShader);
			meshRenderer->loadShaderDepth(GameManager::depthShader);
			meshRenderer->loadMaterial(material);
			meshRenderer->isShaded = false;
			meshRenderer->isColored = true;
			meshRenderer->hasTexture = false;

			transform = lightPoint->getComponent<Transform>();
			transform->position = vec3(-10.0f, 0.0f, 60.0f);

			light = lightPoint->addComponent<Light>();
			light->setType(Light::Type::Directional);
			light->loadShader(GameManager::basicShader);
			light->loadShaderDepth(GameManager::depthShader);
			light->eye = transform->position;
		}

		// player //
		{
			transform = player->getComponent<Transform>();
			transform->position = vec3(-330.7f, 0.f, 0.f);
			transform->scale = vec3(1.f, 1.f, 1.f);
			transform->mass = 1.0f;
			Stage2PlayerScript* playerScript = new Stage2PlayerScript();
			player->addComponent<ScriptLoader>()->addScript(playerScript);

			//create a dynamic rigidbody
			btCollisionShape* colShape = new btSphereShape(btScalar((transform->scale).x));
			collisionShapes.push_back(colShape);

			/// Create Dynamic Objects
			btTransform startTransform;
			startTransform.setIdentity();

			btScalar mass(transform->mass);

			//rigidbody is dynamic if and only if mass is non zero, otherwise static
			bool isDynamic = (mass != 0.f);

			btVector3 localInertia(0, 0, 0);
			if (isDynamic)
				colShape->calculateLocalInertia(mass, localInertia);

			startTransform.setOrigin(btVector3(transform->position.x, transform->position.y, transform->position.z));

			//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
			btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
			CustomRigidBody* body = new CustomRigidBody(rbInfo, objectTypes::PLAYER);

			GameManager::dynamicsWorld->addRigidBody(body);

			transform->body = body;
			body->setLinearVelocity(btVector3(0.0f, 0, 0));
			body->gameObject = player;

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
				meshRenderer->loadShaderDepth(GameManager::depthShader);
				meshRenderer->isShaded = true;
				meshRenderer->hasTexture = true;
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
				meshRenderer->loadShaderDepth(GameManager::depthShader);
				meshRenderer->isShaded = true;
				meshRenderer->isColored = true;
				meshRenderer->color = vec4(1.f);

				transform = playerBack->getComponent<Transform>();
				transform->translate(0.f, 0.f, 0.5f);
				transform->scale = vec3(0.5f, 0.25f, 0.25f);
				meshRenderer = playerBack->addComponent<MeshRenderer>();
				meshRenderer->loadMesh(boxMesh);
				meshRenderer->loadShader(GameManager::basicShader);
				meshRenderer->loadShaderDepth(GameManager::depthShader);
				meshRenderer->isShaded = true;
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
				meshRenderer->loadShaderDepth(GameManager::depthShader);
				meshRenderer->isShaded = true;
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
				meshRenderer->loadShaderDepth(GameManager::depthShader);
				meshRenderer->isShaded = true;
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
				meshRenderer->loadShaderDepth(GameManager::depthShader);
				meshRenderer->isShaded = true;
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
				meshRenderer->loadShaderDepth(GameManager::depthShader);
				meshRenderer->isShaded = true;
				meshRenderer->isColored = true;
				meshRenderer->color = vec4(1.f);
			}
			/*
			meshRenderer = player->addComponent<MeshRenderer>();
			meshRenderer->loadMesh(sphereMesh);
			meshRenderer->loadTexture(meteorTexture);
			meshRenderer->loadShader(GameManager::basicShader);
			meshRenderer->loadShaderDepth(GameManager::depthShader);
			meshRenderer->loadMaterial(material);
			meshRenderer->isShaded = true;
			meshRenderer->isColored = false;
			meshRenderer->hasTexture = true;
			*/
		}

		// walls
		{
			// outer wall
			addObject(createWall(vec3(-300.0f, 60.0f, 0.0f), 60.f, vec3(61.5f, 3.f, 10.f)));
			addObject(createWall(vec3(-300.0f, -60.0f, 0.0f), -60.f, vec3(61.5f, 3.f, 10.f)));
			addObject(createWall(vec3(300.0f, 60.0f, 0.0f), -60.f, vec3(61.5f, 3.f, 10.f)));
			addObject(createWall(vec3(300.0f, -60.0f, 0.0f), 60.f, vec3(61.5f, 3.f, 10.f)));
			addObject(createWall(vec3(0.0f, 72.f, 0.0f), 0.f, vec3(300.0f, 3.f, 10.f)));
			addObject(createWall(vec3(0.0f, -72.f, 0.0f), 0.f, vec3(300.0f, 3.f, 10.f)));

			// middle wall
			addObject(createCylinderWall(vec3(0.0f, -75.3f, 0.f), vec3(70.f, 70.f, 10.f), true));
			addObject(createCylinderWall(vec3(-21.6f, 70.2f, 0.f), vec3(35.f, 35.f, 10.f), true));
			addObject(createWall(vec3(10.0f, 70.f, 0.0f), 90.f, vec3(40.f, 3.f, 10.f)));
			addObject(createWall(vec3(-10.0f, 70.f, 0.0f), 90.f, vec3(40.f, 3.f, 10.f)));
			addObject(createWall(vec3(0.0f, -34.4f, 0.0f), 90.f, vec3(40.f, 3.f, 10.f)));
			addObject(createWall(vec3(-7.05f, 33.05f, 0.0f), 45.f, vec3(17.45f, 3.f, 10.f)));
			addObject(createWall(vec3(7.05f, 33.05f, 0.0f), -45.f, vec3(17.45f, 3.f, 10.f)));
			addObject(createWall(vec3(4.11f, 9.21f, 0.0f), 45.f, vec3(7.75f, 3.f, 10.f)));
			addObject(createWall(vec3(-4.11f, 9.21f, 0.0f), -45.f, vec3(7.75f, 3.f, 10.f)));

			// Rocks (Left)
			addObject(createSphereWall(vec3(-31.9f, 2.8f, 0.f), 10.f));
			addObject(createSphereWall(vec3(-45.1f, 29.1f, 0.f), 5.f));
			addObject(createSphereWall(vec3(-63.1f, 11.4f, 0.f), 5.f));
			addObject(createSphereWall(vec3(-66.f, 59.3f, 0.f), 5.f));
			addObject(createSphereWall(vec3(-99.5f, 38.4f, 0.f), 5.f));
			addObject(createSphereWall(vec3(-121.7f, 21.1f, 0.f), 5.f));
			addObject(createSphereWall(vec3(-114.5f, -36.f, 0.f), 5.f));
			addObject(createSphereWall(vec3(-197.2f, -9.8f, 0.f), 20.f));
			addObject(createSphereWall(vec3(-204.1f, -46.9f, 0.f), 5.f));
			addObject(createSphereWall(vec3(-208.f, 43.4f, 0.f), 5.f));
			addObject(createSphereWall(vec3(-243.1f, 30.2f, 0.f), 12.f));
			addObject(createSphereWall(vec3(-264.5f, -42.1f, 0.f), 12.f));
			addObject(createSphereWall(vec3(-279.f, 46.f, 0.f), 10.f));

			addObject(createSphereWall(vec3(-31.0f, 21.2f, 0.f), 5.f, true));
			addObject(createSphereWall(vec3(-49.8f, 14.6f, 0.f), 5.f, true));
			addObject(createSphereWall(vec3(-52.6f, -6.8f, 0.f), 5.f, true));
			addObject(createSphereWall(vec3(-70.4f, 35.2f, 0.f), 12.f, true));
			addObject(createSphereWall(vec3(-88.3f, 2.4f, 0.f), 10.f, true));
			addObject(createSphereWall(vec3(-132.4f, 47.6f, 0.f), 5.f, true));
			addObject(createSphereWall(vec3(-157.4f, -22.85f, 0.f), 10.f, true));

			// Rocks (Right)
			addObject(createCylinderWall(vec3(104.6f, 21.f, 0.f), vec3(35.f, 35.f, 10.f), true));
			addObject(createCylinderWall(vec3(156.6f, -52.3f, 0.f), vec3(35.f, 35.f, 10.f), true));
			addObject(createWall(vec3(52.f, 50.1f, 0.f), -60.f, vec3(40.f, 3.f, 10.f), true));
			addObject(createWall(vec3(155.1f, 55.2f, 0.f), 60.f, vec3(40.f, 3.f, 10.f), true));
			addObject(createWall(vec3(201.6f, -71.5f, 0.f), -60.f, vec3(40.f, 3.f, 10.f), true));
			addObject(createWall(vec3(109.f, -74.5f, 0.f), 60.f, vec3(40.f, 3.f, 10.f), true));
			addObject(createCylinderWall(vec3(252.9f, 42.4f, 0.f), vec3(50.f, 50.f, 10.f), true));
			addObject(createSphereWall(vec3(218.3f, -30.5f, 0.f), 10.f, true));
			addObject(createSphereWall(vec3(272.4f, -47.6f, 0.f), 15.f, true));
		}

		{
			meshRenderer = backBox->addComponent<MeshRenderer>();
			meshRenderer->loadMesh(boxMesh);
			meshRenderer->loadTexture(whiteTexture);
			meshRenderer->loadMaterial(material);
			meshRenderer->loadShader(GameManager::basicShader);
			meshRenderer->loadShaderDepth(GameManager::depthShader);

			meshRenderer->isShaded = true;
			meshRenderer->isColored = false;
			meshRenderer->hasTexture = true;
			meshRenderer->hasAlpha = false;
			meshRenderer->color = vec4(1.0f, 1.0f, 1.0f, 1.0f);

			transform = backBox->getComponent<Transform>();
			transform->position = vec3(-3.0f, 0.0f, -30.0f);
			transform->rotation = Quaternion(0.f, 0.f, 0.f, 1.f);
			transform->scale = vec3(50.0f, 50.0f, 3.0f);
			transform->mass = 1.0f;

			EmptyBoxScript* emptyBoxScript = new EmptyBoxScript();
			backBox->addComponent<ScriptLoader>()->addScript(emptyBoxScript);
		}

		// save point 1
		{
			meshRenderer = savePoint_1->addComponent<MeshRenderer>();
			meshRenderer->loadMesh(sphereMesh);
			meshRenderer->loadMaterial(material);
			meshRenderer->loadTexture(meteorTexture);
			meshRenderer->loadShader(GameManager::basicShader);
			meshRenderer->loadShaderDepth(GameManager::depthShader);

			meshRenderer->isShaded = true;
			meshRenderer->isColored = true;
			meshRenderer->hasTexture = false;
			meshRenderer->hasAlpha = false;
			meshRenderer->color = vec4(0.2f, 0.9f, 0.2f, 0.6f);

			transform = savePoint_1->getComponent<Transform>();
			transform->position = vec3(9.0f, 0.0f, 0.0f);
			transform->rotation = Quaternion(0.f, 0.f, 0.f, 1.f);
			transform->scale = vec3(0.6f, 0.6f, 0.6f);
			transform->mass = 0.0f;

			SavePointScript* savePointScript = new SavePointScript();
			savePoint_1->addComponent<ScriptLoader>()->addScript(savePointScript);

			soundPlayer = savePoint_1->addComponent<SoundPlayer>();
			soundPlayer->loadSoundFrom("sounds/savePoint.mp3");
			soundPlayer->setType(SoundPlayer::Type::Event2D);
			savePointScript->hasSound = true;

			btCollisionShape* colShape = new btSphereShape(btScalar((transform->scale).x));
			/// Create Dynamic Objects
			btTransform startTransform;
			startTransform.setIdentity();
			btScalar mass(transform->mass);
			//rigidbody is dynamic if and only if mass is non zero, otherwise static
			bool isDynamic = (transform->mass != 0.f);
			btVector3 localInertia(0, 0, 0);
			if (isDynamic)
				colShape->calculateLocalInertia(transform->mass, localInertia);
			startTransform.setOrigin(btVector3(transform->position.x, transform->position.y, transform->position.z));

			//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
			btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
			CustomRigidBody* body = new CustomRigidBody(rbInfo, objectTypes::SAVE_POINT);

			GameManager::dynamicsWorld->addRigidBody(body);

			transform->body = body;
			body->setLinearVelocity(btVector3(0.f, 0, 0));
			body->gameObject = savePoint_1;
		}

		// meteor //
		{
			meteorMoveEventId = EventManager<MeteorMoveEvent>::addListener([this](const MeteorMoveEvent& e)->bool {
				MeteorShowerMaker(e.bottomLeft, e.range, e.meteorNum, e.direction, e.speed, e.randomVelocity, e.randomScale);
				return true;
				});
		}

		// GUI
		gui->addComponent<ScriptLoader>()->addScript(new Stage2GUIScript());
		soundPlayer = gui->addComponent<SoundPlayer>();
	}

	GameObject* createWall(vec3 pos, float rotAngle, vec3 scale, bool radioactive = false) {
		//Material* material = ResourceManager::getMaterial("Basic");
		Mesh* boxMesh = ResourceManager::getMesh("Box");

		// static object test
		Quaternion q = Quaternion::axisAngle(vec3(0.f, 0.f, 1.f), rotAngle);
		btQuaternion btq = btQuaternion(q.x, q.y, q.z, q.w);
		btVector3 btpos = btVector3(pos.x, pos.y, pos.z);
		btVector3 btscale = btVector3(scale.x, scale.y, scale.z);

		GameObject* wall = GameObject::create("Wall");

		MeshRenderer* meshRenderer = wall->addComponent<MeshRenderer>();
		meshRenderer->loadMesh(boxMesh);
		meshRenderer->loadTexture((radioactive ? radioactiveTexture : wallTexture));
		meshRenderer->loadShader(GameManager::basicShader);
		//meshRenderer->loadMaterial(material);
		meshRenderer->isShaded = true;
		meshRenderer->isColored = false;
		meshRenderer->hasTexture = true;

		Transform* transform = wall->getComponent<Transform>();
		transform->position = pos;
		transform->scale = scale;
		transform->rotation = q;

		//create a dynamic rigidbody
		btCollisionShape* colShape = new btBoxShape(btscale);
		collisionShapes.push_back(colShape);

		// Create Dynamic Objects
		btTransform startTransform;
		startTransform.setIdentity();

		btScalar mass(0.f);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			colShape->calculateLocalInertia(mass, localInertia);

		startTransform.setOrigin(btpos);
		startTransform.setRotation(btq);

		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
		CustomRigidBody* body = new CustomRigidBody(rbInfo, (radioactive ? objectTypes::RADIOACTIVE_WALL : objectTypes::WALL));

		GameManager::dynamicsWorld->addRigidBody(body);

		transform->body = body;
		body->setLinearVelocity(btVector3(0.f, 0.f, 0.f));
		body->gameObject = wall;

		return wall;
	}

	GameObject* createCylinderWall(vec3 pos, vec3 scale, bool radioactive = false) {
		Mesh* cylinderMesh = ResourceManager::getMesh("Cylinder");

		// static object test
		Quaternion q = Quaternion::axisAngle(vec3(1.f, 0.f, 0.f), 90.f);
		btQuaternion btq = btQuaternion(q.x, q.y, q.z, q.w);
		btVector3 btpos = btVector3(pos.x, pos.y, pos.z);
		btVector3 btscale = btVector3(scale.x, scale.z, scale.y);

		GameObject* wall = GameObject::create("Radioactive Wall");

		MeshRenderer* meshRenderer = wall->addComponent<MeshRenderer>();
		meshRenderer->loadMesh(cylinderMesh);
		meshRenderer->loadTexture((radioactive ? radioactiveTexture : wallTexture));
		meshRenderer->loadShader(GameManager::basicShader);
		//meshRenderer->loadMaterial(material);
		meshRenderer->isShaded = false;
		meshRenderer->isColored = false;
		meshRenderer->hasTexture = true;

		Transform* transform = wall->getComponent<Transform>();
		transform->position = pos;
		transform->scale = vec3(scale.x, scale.z, scale.y);
		transform->rotation = q;

		//create a dynamic rigidbody
		btCollisionShape* colShape = new btCylinderShape(btscale);
		collisionShapes.push_back(colShape);

		// Create Dynamic Objects
		btTransform startTransform;
		startTransform.setIdentity();

		btScalar mass(0.f);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			colShape->calculateLocalInertia(mass, localInertia);

		startTransform.setOrigin(btpos);
		startTransform.setRotation(btq);

		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
		CustomRigidBody* body = new CustomRigidBody(rbInfo, (radioactive ? objectTypes::RADIOACTIVE_WALL : objectTypes::WALL));

		GameManager::dynamicsWorld->addRigidBody(body);

		transform->body = body;
		body->setLinearVelocity(btVector3(0.f, 0.f, 0.f));
		body->gameObject = wall;

		return wall;
	}

	GameObject* createSphereWall(vec3 pos, float scale, bool radioactive = false) {
		Mesh* cylinderMesh = ResourceManager::getMesh("Sphere");

		// static object test
		Quaternion q = Quaternion::axisAngle(vec3(1.f, 0.f, 0.f), 90.f);
		btQuaternion btq = btQuaternion(q.x, q.y, q.z, q.w);
		btVector3 btpos = btVector3(pos.x, pos.y, pos.z);

		GameObject* wall = GameObject::create("Radioactive Wall");

		MeshRenderer* meshRenderer = wall->addComponent<MeshRenderer>();
		meshRenderer->loadMesh(cylinderMesh);
		meshRenderer->loadTexture((radioactive ? radioactiveTexture : wallTexture));
		meshRenderer->loadShader(GameManager::basicShader);
		//meshRenderer->loadMaterial(material);
		meshRenderer->isShaded = false;
		meshRenderer->isColored = false;
		meshRenderer->hasTexture = true;

		Transform* transform = wall->getComponent<Transform>();
		transform->position = pos;
		transform->scale = vec3(scale, scale, scale);
		transform->rotation = q;

		//create a dynamic rigidbody
		btCollisionShape* colShape = new btSphereShape(btScalar(scale));
		collisionShapes.push_back(colShape);

		// Create Dynamic Objects
		btTransform startTransform;
		startTransform.setIdentity();

		btScalar mass(0.f);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			colShape->calculateLocalInertia(mass, localInertia);

		startTransform.setOrigin(btpos);
		startTransform.setRotation(btq);

		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
		CustomRigidBody* body = new CustomRigidBody(rbInfo, (radioactive ? objectTypes::RADIOACTIVE_WALL : objectTypes::WALL));

		GameManager::dynamicsWorld->addRigidBody(body);

		transform->body = body;
		body->setLinearVelocity(btVector3(0.f, 0.f, 0.f));
		body->gameObject = wall;

		return wall;
	}
};
