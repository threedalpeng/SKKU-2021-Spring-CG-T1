#pragma once
#include "engine/Core.h"

class PlayerAnimationScript : public Script
{
public:
	PlayerAnimationScript() : Script() {}
	Transform* player = nullptr;
	Transform* leftArmAxis = nullptr;
	Transform* rightArmAxis = nullptr;
	Transform* leftLegAxis = nullptr;
	Transform* rightLegAxis = nullptr;
private:

	float legAngleRange = 25.f;
	float legSpeed = 6.f;
	float armSpeed = 150.f;

	float angle = 0.f;
	float acc = 0.f;
public:
	void init() {
		getComponent<Transform>()->rotation = Quaternion::axisAngle(vec3(0.f, 0.f, 1.f), angle * 180.f / PI);
	}

	void update() {
		/*
		vec3 targetPos = vec3();
		if (Input::getKey(GLFW_KEY_DOWN)) {
			targetPos.y -= 1.f;
		}
		if (Input::getKey(GLFW_KEY_UP)) {
			targetPos.y += 1.f;
		}
		if (Input::getKey(GLFW_KEY_RIGHT)) {
			targetPos.x += 1.f;
		}
		if (Input::getKey(GLFW_KEY_LEFT)) {
			targetPos.x -= 1.f;
		}

		if (targetPos != vec3()) {
			targetPos = targetPos.normalize();
			float angle = atan2f(targetPos.y, targetPos.x);
			getComponent<Transform>()->rotation = Quaternion::axisAngle(vec3(0.f, 0.f, 1.f), angle * 180.f / PI);
		}
		*/

		btVector3 btv = player->getVelocityBT();
		vec3 vel = vec3(btv.x(), btv.y(), 0.f);
		if (vel != vec3()) {
			vel = vel.normalize();
			float angle = atan2f(vel.y, vel.x);
			getComponent<Transform>()->rotation = Quaternion::axisAngle(vec3(0.f, 0.f, 1.f), angle * 180.f / PI);
		}
		else {
			getComponent<Transform>()->rotate(vec3(1.f, 0.f, 0.f), Time::delta() * 90.f);
		}

		angle = Time::delta() * legSpeed;
		acc += angle;
		if (acc > 360.f) acc -= 360.f;
		float legSpread = legAngleRange * cosf(acc);

		leftArmAxis->rotate(vec3(0.f, 1.f, 0.f), Time::delta() * armSpeed);
		rightArmAxis->rotate(vec3(0.f, 1.f, 0.f), Time::delta() * armSpeed);
		leftLegAxis->rotation = Quaternion::axisAngle(vec3(0.f, 1.f, 0.f), legSpread);
		rightLegAxis->rotation = Quaternion::axisAngle(vec3(0.f, 1.f, 0.f), -legSpread);
	}

	void printVec3(vec3 v) {
		std::cout << v.x << ", " << v.y << ", " << v.z;
	}
};
