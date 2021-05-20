#pragma once
#include "engine/Core.h"
//*******************************************************************
// bullet3
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

class PlayerScript : public Script
{
public:
	PlayerScript() : Script() {}

private:
	Transform* transform = nullptr;

public:

	void init() override {
		transform = getComponent<Transform>();
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
			transform->setWorlPositionBT(btVector3(-3.0f, 0, 0));
		}
		transform->addVelocityBT(addVelocity);
	}

	void collide() {
	}
};