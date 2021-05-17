#pragma once
#include "engine/Core.h"
#include <iostream>
class ObstacleScript : public Script
{
public:
	ObstacleScript() : Script() {}
	ObstacleScript(vec3 velocity) : Script() { _velocity = velocity; }
	vec3 _velocity = vec3(0, 0, 0);
	bool hasSound = false;

private:
	Transform* transform = nullptr;
	bool leave = true;

public:

	void init() override {
		transform = getComponent<Transform>();
	}

	void update() override {
		vec3 distance = _velocity * Time::delta();
		transform->translate(distance);
	}

	void collide()
	{
		// getComponent<SoundPlayer>()->play();
		// std::cout << "collide meteor " << getComponent<SoundPlayer>() << std::endl;
		// std::cout << "collide meteor " << soundPlayer << std::endl;

		// soundPlayer->play();
		if (leave)
		{
			explode();
			leave = false;
		}
	}

	void explode()
	{
		if (hasSound)
			getComponent<SoundPlayer>()->play();
	}
};