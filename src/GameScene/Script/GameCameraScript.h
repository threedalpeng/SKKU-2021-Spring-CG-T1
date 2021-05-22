#pragma once
#include <iostream>
#include "engine/Core.h"

class GameCameraScript : public Script
{
public:
	GameCameraScript() : Script() {}
	Transform* target = nullptr;

private:
	Camera* camera = nullptr;

	float distance = 20.f;
	float rangeRadius = 0.f;
	float rangeAngle = 25.f * PI / 180.f;

public:
	void init() override {
		getComponent<Transform>()->translate(vec3(0, 0, distance));
		camera = getComponent<Camera>();
		// camera->at = vec3(0, 0, 0);
		target = getObject()->getParent()->getComponent<Transform>();
		camera->activateTracking(target);
		rangeRadius = tanf(rangeAngle) * distance;
	}

	void update() override {
		vec3 currentPos = getComponent<Transform>()->position;
		vec3 originPos = vec3(0.f, 0.f, distance);
		vec3 targetPos = vec3();
		if (Input::getKey(GLFW_KEY_DOWN)) {
			targetPos.y += 1.f;
		}
		if (Input::getKey(GLFW_KEY_UP)) {
			targetPos.y -= 1.f;
		}
		if (Input::getKey(GLFW_KEY_RIGHT)) {
			targetPos.x -= 1.f;
		}
		if (Input::getKey(GLFW_KEY_LEFT)) {
			targetPos.x += 1.f;
		}

		targetPos = (targetPos != vec3() ? targetPos.normalize() : targetPos) * rangeRadius + originPos;
		vec3 drt = targetPos - currentPos;
		getComponent<Transform>()->translate(Time::delta() * drt);
	}
};
