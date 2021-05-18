#pragma once
#include <iostream>
#include "engine/Core.h"

class CameraScript : public Script
{
public:
	CameraScript() : Script() {}

private:
	Camera* camera = nullptr;

public:

	void init() override {
		getComponent<Transform>()->translate(vec3(0, 0, 10));
		camera = getComponent<Camera>();
		camera->at = vec3(0, 0, 0);
	}

	void update() override {
	}
};
