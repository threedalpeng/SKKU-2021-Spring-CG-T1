#pragma once
#include "engine/Core.h"
#include "EventCheckerBox.h"
//*******************************************************************
// bullet3
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#include "../../Custom/CustomRigidBody.h"

#include <iostream>

class PlayerScript : public Script
{
public:
	PlayerScript() : Script() {}
	btVector3	savePoint = btVector3(-3.0f, 0, 0);
	int 	HP = 100;
	float 	lastWallCollistion = 0.0f;

private:
	Transform* transform = nullptr;
	//EventCheckerBox gravityFallEvent();
public:

	void init() override {
		transform = getComponent<Transform>();
	}

	void update() override {
		/*
		if (9.f < transform->worldPosition.x && transform->worldPosition.x < 10.f) {
			GameManager::dynamicsWorld->setGravity(btVector3(0.f, 7.f, 0.f));
		}
		if (15.f < transform->worldPosition.x && transform->worldPosition.x < 16.f) {
			GameManager::dynamicsWorld->setGravity(btVector3(0.f, 0.f, 0.f));
		}
		*/

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
		}
		else if(Input::getKeyDown(GLFW_KEY_SPACE))
		{

		}
		transform->addVelocityBT(addVelocity);
		lastWallCollistion = std::max(0.0f, lastWallCollistion - Time::delta());
	}

	void collide(objectTypes oppositeType) {
		if(oppositeType == objectTypes::METEOR){
			HP -= 10;
			std::cout << "Now player's HP is " << HP << "\r";
		}		
		else if(oppositeType == objectTypes::WALL && lastWallCollistion < 0.1f){
			HP -= 1;
			lastWallCollistion = 1.0f;
			std::cout << "Now player's HP is " << HP << "\r";
		}	
		
	}

	void shot()
	{
		MeshRenderer* meshRenderer;
		Mesh* sphereMesh = MeshMaker::makeSphere();
	}
};