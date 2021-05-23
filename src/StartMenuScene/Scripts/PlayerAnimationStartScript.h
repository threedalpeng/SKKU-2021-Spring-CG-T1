#pragma once
#include "engine/Core.h"

class PlayerAnimationStartScript : public Script
{
public:
	PlayerAnimationStartScript() : Script() {}
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
		getComponent<Transform>()->rotate(vec3(1.f, 0.f, 0.f), 90.f);
	}

	void update() {
		angle = Time::delta() * legSpeed;
		acc += angle;
		if (acc > 360.f) acc -= 360.f;
		float legSpread = legAngleRange * cosf(acc);

		getComponent<Transform>()->rotate(vec3(1.f, 0.f, 0.f), Time::delta() * 90.f);
		leftArmAxis->rotate(vec3(0.f, 1.f, 0.f), Time::delta() * armSpeed);
		rightArmAxis->rotate(vec3(0.f, 1.f, 0.f), Time::delta() * armSpeed);
		leftLegAxis->rotation = Quaternion::axisAngle(vec3(0.f, 1.f, 0.f), legSpread);
		rightLegAxis->rotation = Quaternion::axisAngle(vec3(0.f, 1.f, 0.f), -legSpread);
	}
	void printVec3(vec3 v) {
		std::cout << v.x << ", " << v.y << ", " << v.z;
	}
};
