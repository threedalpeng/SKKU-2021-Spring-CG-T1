#pragma once

// Standard Library
#include <iostream>

// Engine
#include "engine/Core.h"
#include "cg/cgmath.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/GameManager.h"
#include "../GameScene/Script/ObstacleScript.h"

GameObject* createMeteor(vec3 pos, vec3 velocity, float scale) {
	Texture* meteorTexture = nullptr;
	meteorTexture = ResourceManager::getTexture("venus");
	btAlignedObjectArray<btCollisionShape*> collisionShapes;

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

void MeteorShowerMaker(vec3 anchorPoint, vec2 rangeXY, int meteourNum, vec3 direction, float speed, bool randomVelocity = false, bool randomScale = false)
{
	/*
	anchorPoint: left bottom posion of field where meteors is created.
	*/
	GameObject* meteor;
	for (int i = 0; i < meteourNum; i++)
	{
		vec3 pos = vec3(0);
		// pos.x = anchorPoint.x + GameManager::genRandFloat() * rangeXY.x;
		pos.x = anchorPoint.x + rangeXY.x * (i / (float)meteourNum);
		pos.y = anchorPoint.y + ((GameManager::genRandFloat() + 1.f) * rangeXY.y / 2.f);
		pos.z = 0.0f;

		vec3 velocity = vec3(0);
		float speed = 9.0f;
		if (direction.length() > 0.0f) velocity = direction.normalize() * speed;
		else velocity = direction * speed;

		if (randomVelocity)
		{
			vec3 noise = vec3(0);
			float noiseScale = 1.0f;
			noise.x = GameManager::genRandFloat() * noiseScale;
			noise.y = GameManager::genRandFloat() * noiseScale;
			velocity += noise;
		}

		float scale = 1.0f;
		if (randomScale)
		{
			float noiseScale = 1.0f;
			float noise = (GameManager::genRandFloat() + 1.f) * noiseScale;
			scale += noise;
		}

		meteor = createMeteor(pos, velocity, scale);
		meteor->getComponent<ScriptLoader>()->getScripts()[0]->init();
		SceneManager::scene()->addObject(meteor);
	}
}