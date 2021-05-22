#pragma once

#include "engine/Core.h"
#include "EventCheckerSphere.h"
//*******************************************************************
// bullet3
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#include "../../Custom/CustomRigidBody.h"

#include "BulletScript.h"

#include <iostream>

class PlayerScript : public Script
{
public:
	PlayerScript() : Script() {}
	~PlayerScript() {
		EventManager<GuiEvent>::removeListener(guiEventId);
	}

	int HP = 100;
	btVector3 savePoint = btVector3(-3.0f, 0, 0);
	float lastWallCollistion = 0.0f;
	float lastShot = 1.0f;

private:
	Transform* transform = nullptr;
	int eventProgress = 0;
	int saveEventProgress = 0;
	std::vector<EventCheckerSphere> eventCheckers = {
		EventCheckerSphere(vec3(31.19f, -64.36f, 0.0f), 5.f, [this]() {
			stopPlayer();
			EventManager<GuiEvent>::triggerEvent({ 4 }); // Monolog 1
			}),
		EventCheckerSphere(vec3(81.12f, -59.12f, 0.0f), 5.f, [this]() {
			stopPlayer();
			EventManager<GuiEvent>::triggerEvent({ 5 }); // Monolog 2
			gravityOn = true;
			}),
		EventCheckerSphere(vec3(81.12f, 25.7f, 0.0f), 5.f, [this]() {
			GameManager::dynamicsWorld->setGravity(btVector3(0.f, 0.f, 0.f));
			}),
		EventCheckerSphere(vec3(100.6f, 30.2f, 0.f), 8.5f, [this]() {
			stopPlayer();
			savePoint = btVector3(100.6f, 30.2f, 0.f);
			saveEventProgress = eventProgress + 1;
			EventManager<GuiEvent>::triggerEvent({ 6 }); // Monolog 3
			}),
		EventCheckerSphere(vec3(247.8f, 35.7f, 0.f), 15.f, [this]() {
			stopPlayer();
			EventManager<GuiEvent>::triggerEvent({ 7 }); // Monolog 4
			}),
		EventCheckerSphere(vec3(324.95f, 35.7f, 0.f), 15.f, [this]() {
			stopPlayer();
			EventManager<GuiEvent>::triggerEvent({ 8 }); // End
			}),
	};

	uint guiEventId = 0;

	bool gameStopped = false;
	bool gravityOn = false;

public:

	bool guiFinished(const GuiEvent& e) {
		if (e.guiId == -1) // if Enter to Game Mode
			gameStopped = false;
		if (gravityOn) {
			GameManager::dynamicsWorld->setGravity(btVector3(0.f, 7.f, 0.f));
			gravityOn = false;
		}
		return true;
	}

	void init() override {
		transform = getComponent<Transform>();
		guiEventId = EventManager<GuiEvent>::addListener([this](const GuiEvent& e) -> bool {
			return guiFinished(e);
			});
	}

	void update() override {
		//printf("%f, %f\n", transform->worldPosition.x, transform->worldPosition.y);
		if (gameStopped) {
			return;
		}
		for (size_t i = eventProgress; i < eventCheckers.size(); i++) {
			if (eventCheckers[i].trigger(transform->worldPosition))
				eventProgress++;
			else break;
		}

		btVector3 addVelocity = btVector3(0, 0, 0);
		btVector3 currentVelocity = transform->getVelocityBT();
		if (currentVelocity.getX() >= -8.0f && Input::getKey(GLFW_KEY_LEFT))  addVelocity.setX(-8.0f * Time::delta());
		else if (currentVelocity.getX() <= 8.0f && Input::getKey(GLFW_KEY_RIGHT))  addVelocity.setX(+8.0f * Time::delta());
		if (currentVelocity.getY() >= -8.0f && Input::getKey(GLFW_KEY_DOWN))   addVelocity.setY(-8.0f * Time::delta());
		else if (currentVelocity.getY() <= 8.0f && Input::getKey(GLFW_KEY_UP))  addVelocity.setY(+8.0f * Time::delta());

		if (Input::getKeyDown(GLFW_KEY_R))
		{
			// _velocity = vec3(0);
			// transform->position = vec3(-3.0f, 0.0f, 0.0f);
			transform->setVelocityBT(btVector3(0, 0, 0));
			transform->setWorlPositionBT(savePoint);
			HP = 100;
			EventManager<HpChangedEvent>::triggerEvent({ HP });
			eventProgress = saveEventProgress;
			for (size_t i = eventProgress; i < eventCheckers.size(); i++) {
				eventCheckers[i].retry();
			}
		}
		else if (Input::getKeyDown(GLFW_KEY_SPACE))
		{
		}
		transform->addVelocityBT(addVelocity);
		lastWallCollistion = std::max(0.0f, lastWallCollistion - Time::delta());

		// shot
		lastShot = std::max(0.0f, lastShot - Time::delta());
		if (Input::getKey(GLFW_KEY_SPACE) && lastShot < 0.1f)  shot();
	}

	void collide(objectTypes oppositeType) {
		if (oppositeType == objectTypes::METEOR) {
			HP -= 10;
			EventManager<HpChangedEvent>::triggerEvent({ HP });
		}
		else if (oppositeType == objectTypes::WALL && lastWallCollistion < 0.1f) {
			HP -= 1;
			lastWallCollistion = 1.0f;
			EventManager<HpChangedEvent>::triggerEvent({ HP });
		}
		else if (oppositeType == objectTypes::RADIOACTIVE_WALL && lastWallCollistion < 0.1f) {
			HP -= 20;
			lastWallCollistion = 1.0f;
			EventManager<HpChangedEvent>::triggerEvent({ HP });
		}
		else if (oppositeType == objectTypes::SAVE_POINT) {
			savePoint = btVector3(transform->position.x, transform->position.y, transform->position.z);
		}
	}

	void shot()
	{
		lastShot = 1.0f;
		MeshRenderer* bulletMeshRenderer;
		Mesh* sphereMesh = ResourceManager::getMesh("Sphere");
		Material* material = ResourceManager::getMaterial("Basic");
		Transform* bulletTransform;

		GameObject* bullet = GameObject::create("bullet");

		bulletMeshRenderer = bullet->addComponent<MeshRenderer>();
		bulletMeshRenderer->loadMesh(sphereMesh);
		bulletMeshRenderer->loadMaterial(material);
		bulletMeshRenderer->loadShader(GameManager::basicShader);

		bulletMeshRenderer->isShaded = true;
		bulletMeshRenderer->isColored = true;
		bulletMeshRenderer->hasTexture = false;
		bulletMeshRenderer->hasAlpha = false;
		bulletMeshRenderer->color = vec4(0.5f, 0.5f, 1.0f, 1.0f);

		bulletTransform = bullet->getComponent<Transform>();
		bulletTransform->position = transform->position;
		bulletTransform->rotation = Quaternion(0.f, 0.f, 0.f, 1.f);
		bulletTransform->scale = vec3(0.15f, 0.15f, 0.15f);
		bulletTransform->mass = 0.1f;

		BulletScript* bulletScript = new BulletScript();
		bullet->addComponent<ScriptLoader>()->addScript(bulletScript);
		bulletScript->init();
		bulletScript->_velocity = vec3(
			transform->getVelocityBT().x() + 0.1e-10f, transform->getVelocityBT().y(), transform->getVelocityBT().z()
		).normalize() * 16;
		bulletTransform->position += bulletScript->_velocity.normalize() * 2.5;

		btCollisionShape* colShape = new btSphereShape(btScalar((bulletTransform->scale).x));

		/// Create Dynamic Objects
		btTransform startTransform;
		startTransform.setIdentity();
		btScalar mass(bulletTransform->mass);
		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (bulletTransform->mass != 0.f);
		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			colShape->calculateLocalInertia(bulletTransform->mass, localInertia);
		startTransform.setOrigin(btVector3(bulletTransform->position.x, bulletTransform->position.y, bulletTransform->position.z));

		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
		CustomRigidBody* body = new CustomRigidBody(rbInfo, objectTypes::BULLET);

		GameManager::dynamicsWorld->addRigidBody(body);

		bulletTransform->body = body;
		body->setLinearVelocity(btVector3(bulletScript->_velocity.x, bulletScript->_velocity.y, bulletScript->_velocity.z));
		body->gameObject = bullet;

		SceneManager::scene()->addObject(bullet);
	}

	void stopPlayer() {
		gameStopped = true;
		transform->setVelocityBT(btVector3(0.f, 0.f, 0.f));
	}
};