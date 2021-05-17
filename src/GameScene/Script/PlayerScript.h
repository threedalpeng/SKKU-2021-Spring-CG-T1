#pragma once
#include "engine/Core.h"

class PlayerScript : public Script
{
public:
	PlayerScript() : Script() {}

private:
	Transform* transform = nullptr;
	RigidBody* rigidBody = nullptr;

public:

	void init() override {
		transform = getComponent<Transform>();
		rigidBody = getComponent<RigidBody>();
	}

	void fixedUpdate() override {
		btVector3 addVelocity = btVector3(0, 0, 0);
		if (Input::getKey(GLFW_KEY_DOWN))
			addVelocity.setY(-8.0f * Time::fixedDelta());
		else if (Input::getKey(GLFW_KEY_UP))
			addVelocity.setY(+8.0f * Time::fixedDelta());
		else if (Input::getKey(GLFW_KEY_LEFT))
			addVelocity.setX(-8.0f * Time::fixedDelta());
		else if (Input::getKey(GLFW_KEY_RIGHT))
			addVelocity.setX(+8.0f * Time::fixedDelta());
		if (Input::getKeyDown(GLFW_KEY_HOME)) {
			rigidBody->rigidBody()->setLinearVelocity(btVector3(0.f, 0.f, 0.f));
		}
		rigidBody->rigidBody()->setLinearVelocity(rigidBody->rigidBody()->getLinearVelocity() + addVelocity);
	}

	void update() override {
		std::cout << "Player Update: " << transform->worldPosition.x << " " << transform->worldPosition.y << " " << transform->worldPosition.z << std::endl;
		if (Input::getKeyDown(GLFW_KEY_HOME)) {
			transform->position = vec3();
		}
	}
};