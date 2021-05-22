#pragma once

// Standard Library
#include <iostream>

// Engine
#include "engine/Core.h"

// My Application
#include "Script/ObstacleScript.h"
#include "Script/Stage1PlayerScript.h"
#include "Script/PlayerAnimationScript.h"
#include "Script/GameCameraScript.h"
#include "Script/GameBackgroundScript.h"
#include "Script/Stage1GUIScript.h"
#include "Script/EmptyBoxScript.h"
#include "Script/SavePointScript.h"

#include "../Tool/MeshMaker.h"
#include "../Tool/ParticleMaker.h"
#include "../Custom/CustomRigidBody.h"

//*******************************************************************
// bullet3
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#include "PhysicsUtil.h"

class Stage_1 : public Scene {
public:
	Stage_1() : Scene() {};
	~Stage_1() {
		EventManager<MeteorMoveEvent>::removeListener(meteorMoveEventId);

		for (int i = GameManager::dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
		{
			btCollisionObject* obj = GameManager::dynamicsWorld->getCollisionObjectArray()[i];
			GameManager::dynamicsWorld->removeCollisionObject(obj);
			delete obj;
		}
		for (int j = 0; j < collisionShapes.size(); j++)
		{
			btCollisionShape* shape = collisionShapes[j];
			delete shape;
		}

		//delete dynamics world
		delete GameManager::dynamicsWorld;
		GameManager::dynamicsWorld = nullptr;

		//delete collision algorithms creation functions
		delete m_solver;

		delete m_broadphase;

		delete m_pairCache;

		delete m_dispatcher;

		delete m_filterCallback;

		delete m_collisionConfiguration;
	}

	Texture* wallTexture = nullptr;
	Texture* radioactiveTexture = nullptr;
	Texture* meteorTexture = nullptr;

	btAlignedObjectArray<btCollisionShape*> collisionShapes;
	uint meteorMoveEventId;

private:
	btDefaultCollisionConfiguration* m_collisionConfiguration;
	MyOverlapFilterCallback2* m_filterCallback;
	btCollisionDispatcher* m_dispatcher;
	btOverlappingPairCache* m_pairCache;
	btBroadphaseInterface* m_broadphase;
	btMultiBodyConstraintSolver* m_solver;

public:
	void init() {
		/* Font */
		/*
		Font* textFont = new Font();
		textFont->loadFrom("fonts/consola.ttf");
		*/

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

		GameObject* gui = GameObject::create("GUI");

		//**********************************************
		// bullet init

		m_collisionConfiguration = new btDefaultCollisionConfiguration();
		m_filterCallback = new MyOverlapFilterCallback2();
		m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
		m_pairCache = new btHashedOverlappingPairCache();
		m_pairCache->setOverlapFilterCallback(m_filterCallback);
		m_broadphase = new btDbvtBroadphase(m_pairCache);
		m_solver = new btMultiBodyConstraintSolver;

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
			transform->position = vec3(-3.0f, 0.0f, 0.0f);
			transform->scale = vec3(0.6f, 0.6f, 0.6f);
			transform->mass = 1.0f;
			Stage1PlayerScript* playerScript = new Stage1PlayerScript();
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
			addObject(createWall(vec3(-13.0f, 0.0f, 0.0f), 90.f, vec3(15.f, 3.f, 10.f)));
			addObject(createWall(vec3(+10.0f, 12.0f, 0.0f), 0.f, vec3(20.f, 3.f, 10.f)));
			addObject(createWall(vec3(+10.0f, -12.0f, 0.0f), 0.f, vec3(20.f, 3.f, 10.f)));
			addObject(createWall(vec3(+47.47f, 1.43f, 0.0f), -30.f, vec3(31.35f, 3.f, 9.5f)));
			addObject(createWall(vec3(+32.13f, -10.0f, 0.0f), 30.f, vec3(7.52f, 3.f, 9.75f)));
			addObject(createWall(vec3(44.63f, -11.47f, 0.0f), -30.f, vec3(10.4f, 3.f, 10.f)));
			addObject(createWall(vec3(55.28f, -28.4f, 0.0f), 45.f, vec3(23.3f, 3.f, 10.f)));
			addObject(createWall(vec3(35.68f, -31.46f, 0.0f), 45.f, vec3(20.25f, 3.f, 10.f)));
			addObject(createWall(vec3(29.66f, -49.9f, 0.0f), -45.f, vec3(7.35f, 3.f, 10.f)));
			addObject(createWall(vec3(45.68f, -58.6f, 0.0f), 45.f, vec3(13.25f, 3.f, 10.f)));
			addObject(createWall(vec3(40.f, -81.1f, 0.0f), 45.f, vec3(14.65f, 3.f, 10.f)));
			addObject(createWall(vec3(44.21f, -46.03f, 0.0f), -45.f, vec3(7.35f, 3.f, 10.f)));
			addObject(createWall(vec3(22.46f, -63.2f, 0.0f), 45.f, vec3(13.22f, 3.f, 10.f)));
			addObject(createRadioactiveWall(vec3(23.09f, -81.01f, 0.f), 0.f, vec3(10.f, 10.f, 1.f)));
			addObject(createWall(vec3(60.5f, -59.5f, 0.0f), 0.f, vec3(15.05f, 3.f, 10.f)));
			addObject(createWall(vec3(69.41f, -71.67f, 0.0f), 0.f, vec3(21.2f, 3.f, 10.f)));
			addObject(createWall(vec3(87.4f, -33.44f, 0.0f), 90.f, vec3(56.f, 3.f, 10.f)));
			addObject(createWall(vec3(73.3f, -6.3f, 0.0f), 90.f, vec3(56.f, 3.f, 10.f)));
			addObject(createRadioactiveWall(vec3(80.12f, 52.6f, 0.f), 0.f, vec3(10.f, 10.f, 1.f)));
			addObject(createWall(vec3(194.7f, 52.08f, 0.0f), 0.f, vec3(123.15f, 3.f, 10.f)));
			addObject(createWall(vec3(200.9f, 19.4f, 0.0f), 0.f, vec3(116.5f, 3.f, 10.f)));
			addObject(createRadioactiveWall(vec3(119.7f, 52.08f, 0.f), 0.f, vec3(22.5f, 22.5f, 1.f)));
			addObject(createRadioactiveWall(vec3(160.3f, 19.4f, 0.f), 0.f, vec3(22.5f, 22.5f, 1.f)));
			addObject(createRadioactiveWall(vec3(200.5f, 52.08f, 0.f), 0.f, vec3(22.5f, 22.5f, 1.f)));
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
			collisionShapes.push_back(colShape);
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
				GameObject* meteor;
				meteor = createMeteor(vec3(277.2f, 41.81f, 0.f), vec3(-3.f, 0.f, 0.f), 5.f);
				meteor->getComponent<ScriptLoader>()->getScripts()[0]->init();
				addObject(meteor);
				meteor = createMeteor(vec3(293.6f, 30.05f, 0.f), vec3(-3.f, 0.f, 0.f), 5.f);
				meteor->getComponent<ScriptLoader>()->getScripts()[0]->init();
				addObject(meteor);
				meteor = createMeteor(vec3(314.1f, 36.48f, 0.f), vec3(-3.f, 0.f, 0.f), 10.f);
				meteor->getComponent<ScriptLoader>()->getScripts()[0]->init();
				addObject(meteor);
				return true;
				});
		}

		// GUI
		gui->addComponent<ScriptLoader>()->addScript(new Stage1GUIScript());
		soundPlayer = gui->addComponent<SoundPlayer>();
	}

	GameObject* createWall(vec3 pos, float rotAngle, vec3 scale) {
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
		meshRenderer->loadTexture(wallTexture);
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
		CustomRigidBody* body = new CustomRigidBody(rbInfo, objectTypes::WALL);

		GameManager::dynamicsWorld->addRigidBody(body);

		transform->body = body;
		body->setLinearVelocity(btVector3(0.f, 0.f, 0.f));
		body->gameObject = wall;

		return wall;
	}

	GameObject* createRadioactiveWall(vec3 pos, float rotAngle, vec3 scale) {
		Mesh* cylinderMesh = ResourceManager::getMesh("Cylinder");

		// static object test
		Quaternion q = Quaternion::axisAngle(vec3(1.f, 0.f, 0.f), 90.f);
		btQuaternion btq = btQuaternion(q.x, q.y, q.z, q.w);
		btVector3 btpos = btVector3(pos.x, pos.y, pos.z);
		btVector3 btscale = btVector3(scale.x, scale.z, scale.y);

		GameObject* wall = GameObject::create("Radioactive Wall");

		MeshRenderer* meshRenderer = wall->addComponent<MeshRenderer>();
		meshRenderer->loadMesh(cylinderMesh);
		meshRenderer->loadTexture(radioactiveTexture);
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
		CustomRigidBody* body = new CustomRigidBody(rbInfo, objectTypes::RADIOACTIVE_WALL);

		GameManager::dynamicsWorld->addRigidBody(body);

		transform->body = body;
		body->setLinearVelocity(btVector3(0.f, 0.f, 0.f));
		body->gameObject = wall;

		return wall;
	}

	GameObject* createMeteor(vec3 pos, vec3 velocity, float scale) {
		Texture* meteorTexture = nullptr;
		meteorTexture = ResourceManager::getTexture("venus");

		GameObject* meteor = GameObject::create("Meteor");

		MeshRenderer* meshRenderer = meteor->addComponent<MeshRenderer>();
		meshRenderer->loadMesh(ResourceManager::getMesh("Sphere"));
		meshRenderer->loadTexture(meteorTexture);
		meshRenderer->loadShader(GameManager::basicShader);
		meshRenderer->loadShaderDepth(GameManager::depthShader);
		meshRenderer->loadMaterial(ResourceManager::getMaterial("Basic"));
		meshRenderer->isShaded = true;
		meshRenderer->isColored = false;
		meshRenderer->hasTexture = true;

		Transform* transform = meteor->getComponent<Transform>();
		transform->position = vec3(pos);
		transform->scale = vec3(scale);
		ObstacleScript* obstacleScript = new ObstacleScript(velocity);
		obstacleScript->hasSound = true;
		meteor->addComponent<ScriptLoader>()->addScript(obstacleScript);

		//create a dynamic rigidbody
		btCollisionShape* colShape = new btSphereShape(btScalar(transform->scale.x));
		collisionShapes.push_back(colShape);

		// Create Dynamic Objects
		btTransform startTransform;
		startTransform.setIdentity();

		btScalar mass(scale);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			colShape->calculateLocalInertia(mass, localInertia);

		startTransform.setOrigin(btVector3(pos.x, pos.y, pos.z));

		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
		CustomRigidBody* body = new CustomRigidBody(rbInfo, objectTypes::METEOR);

		GameManager::dynamicsWorld->addRigidBody(body);

		transform->body = body;
		body->setLinearVelocity(btVector3(velocity.x, velocity.y, velocity.z));
		body->gameObject = meteor;

		SoundPlayer* soundPlayer = meteor->addComponent<SoundPlayer>();
		soundPlayer->loadSoundFrom("sounds/explode.mp3");
		soundPlayer->setType(SoundPlayer::Type::Event2D);

		return meteor;
	}
};
