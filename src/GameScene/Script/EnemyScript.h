#pragma once

#include "engine/Core.h"
//*******************************************************************
// bullet3
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#include "../../Custom/CustomRigidBody.h"

#include "BulletScript.h"

#include <iostream>

class EnemyScript : public Script
{
public:
	EnemyScript() : Script() {}
    EnemyScript(float minY, float maxY) : Script(), minY(minY), maxY(maxY)  {}

	int HP = 50;
	float lastShot = 2.0f;
	Transform* axis = nullptr;

    float minY = -3.f, maxY = 3.f;
    float direction = 1.0f;
    float speed = 4.0f;

protected:
	Transform* transform = nullptr;

public:
	void init() override {
		transform = getComponent<Transform>();
		HP = 50;
	}

	void update() override {
        btVector3 btPosition = transform->getWorlPositionBT();
        
        btPosition.setY( btPosition.getY() +  direction * speed * Time::delta());
        btPosition.setZ(0.0f);
        if( btPosition.getY() <= minY ||  maxY <= btPosition.getY())
        {
            direction = (-1) * direction;
            btPosition.setY( btPosition.getY() +  direction * 2 * speed * Time::delta());
        }
		transform->setWorlPositionBT(btPosition);

        
		// shot
        if(lastShot < 0.1f)
        {
            shot(vec3(1, 0, 0), -90.f);
            shot(vec3(1, 1, 0), -45.f);
            shot(vec3(0, 1, 0), 0.f);
            shot(vec3(-1, 1, 0), 45.f);
            shot(vec3(-1, 0, 0), 90.f);
            shot(vec3(-1, -1, 0), 135.f);
            shot(vec3(0, -1, 0), 180.f);
            shot(vec3(1, -1, 0), -135.f);
        } 
		lastShot = std::max(0.0f, lastShot - Time::delta());

        if(HP < 1) disappear();
	}

	void collide(objectTypes oppositeType) {
		if (oppositeType == objectTypes::BULLET) {
			HP -= 10;
            std::cout << "enemy HP is " << HP << std::endl;
		}
	}

    void disappear()
	{
		GameObject* thisObject = getObject();
		btRigidBody* objBody = thisObject->getComponent<Transform>()->body;
		if (objBody)	GameManager::removeBodyList.push_back(objBody);
        GameManager::removeList.push_back(thisObject);
	}

	void shot(vec3 shotDirection, float shotAngle)
	{
		lastShot = 2.0f;
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
		// bulletTransform->rotation = axis->rotation * Quaternion::axisAngle(vec3(0.f, 0.f, 1.f), -90.f);
        bulletTransform->rotation = Quaternion::axisAngle(vec3(0.f, 0.f, 1.f), shotAngle);
		bulletTransform->scale = vec3(0.3f, 0.3f, 0.3f);
		bulletTransform->mass = 0.1f;

		BulletScript* bulletScript = new BulletScript(objectTypes::ENEMY);
		bullet->addComponent<ScriptLoader>()->addScript(bulletScript);
		bulletScript->init();
		// bulletScript->_velocity = vec3(
		// 	transform->getVelocityBT().x() + 0.1e-10f, transform->getVelocityBT().y(), transform->getVelocityBT().z()
		// );
		// if(bulletScript->_velocity.length() > 0.0f) bulletScript->_velocity = bulletScript->_velocity.normalize() * 16;
        bulletScript->_velocity = (shotDirection.length() >0.0f ? shotDirection.normalize() * 16.f : vec3(-16.f, 0, 0) );
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
		CustomRigidBody* body = new CustomRigidBody(rbInfo, objectTypes::ENEMY_BULLET);

		GameManager::dynamicsWorld->addRigidBody(body);

		bulletTransform->body = body;
		body->setLinearVelocity(btVector3(bulletScript->_velocity.x, bulletScript->_velocity.y, bulletScript->_velocity.z));
		body->gameObject = bullet;

		SceneManager::scene()->addObject(bullet);
	}


};