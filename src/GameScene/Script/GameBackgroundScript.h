#pragma once
#include "engine/Core.h"

class GameBackgroundScript : public Script
{
public:
	GameBackgroundScript() : Script() {}

private:
	Transform* transform = nullptr;

public:

	void init() override {
		transform = getComponent<Transform>();
		//getComponent<SoundPlayer>()->play();
	}

	void update() override {
		float angle = 5.f * Time::delta();
		transform->rotate(vec3(0, 1, 0), angle);
	}
};