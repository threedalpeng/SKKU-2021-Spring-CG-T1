#pragma once
#include <iostream>
#include "engine/Core.h"

class DepthCameraScript : public Script
{
public:
	DepthCameraScript() : Script() {}
	Transform* target = nullptr;
	GameObject* light = nullptr;


private:
	Camera* camera = nullptr;

	float distance = 10.f;
	float rangeRadius = 0.f;
	float rangeAngle = 20.f * PI / 180.f;

public:
	void init() override {
		getComponent<Transform>()->translate(vec3(0, 0, distance));
		camera = getComponent<Camera>();
		camera->at = vec3(0, 0, 0);
		rangeRadius = tanf(rangeAngle) * distance;
	}

	void update() override {
		camera->eye = light->getComponent<Transform>()->position;
		camera->at = Camera::main->at;
	}
};
