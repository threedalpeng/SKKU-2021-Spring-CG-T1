#pragma once
#pragma once

#include "engine/Core.h"
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

	int HP = 100;
	btVector3 savePoint = btVector3(-3.0f, 0, 0);
	float lastWallCollistion = 0.0f;
	float lastShot = 1.0f;
	Transform* axis = nullptr;

protected:
	Transform* transform = nullptr;
	bool gameStopped = false;

public:
	void init() override {
		transform = getComponent<Transform>();
		HP = 100;
	}

	void update() override {
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
			// eventProgress = saveEventProgress;
			// for (size_t i = eventProgress; i < eventCheckers.size(); i++) {
			// 	eventCheckers[i].retry();
			// }
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
		Transform* bulletTransform;

		GameObject* bullet = GameObject::create("bullet");

		bulletMeshRenderer = bullet->addComponent<MeshRenderer>();
		bulletMeshRenderer->loadMesh(ResourceManager::getMesh("Bullet"));
		bulletMeshRenderer->loadMaterial(ResourceManager::getMaterial("Basic"));
		bulletMeshRenderer->loadTexture(ResourceManager::getTexture("bullet"));
		bulletMeshRenderer->loadShader(GameManager::basicShader);

		bulletMeshRenderer->isShaded = true;
		bulletMeshRenderer->isColored = true;
		bulletMeshRenderer->hasTexture = false;
		bulletMeshRenderer->hasAlpha = false;
		bulletMeshRenderer->color = vec4(1.0f, 0.5f, 1.0f, 1.0f);

		bulletTransform = bullet->getComponent<Transform>();
		bulletTransform->position = transform->position;
		bulletTransform->rotation = axis->rotation * Quaternion::axisAngle(vec3(0.f, 0.f, 1.f), -90.f); // ! bug
		bulletTransform->scale = vec3(0.3f, 0.3f, 0.3f);
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