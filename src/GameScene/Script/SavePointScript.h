#pragma once
#include "engine/Core.h"
#include <iostream>

#include "../../Tool/ParticleMaker.h"
#include "../../Custom/CustomRigidBody.h"
class SavePointScript : public Script
{
public:
	SavePointScript() : Script() {}
	SavePointScript(vec3 velocity) : Script() { _velocity = velocity; }
	~SavePointScript() {}
	vec3 _velocity = vec3(0, 0, 0);
	bool hasSound = false;
	float lastSave = 2.0f;

private:
	Transform* transform = nullptr;

public:

	void init() override
	{
		transform = getComponent<Transform>();
	}

	void update() override
	{
		vec3 distance = _velocity * Time::delta();
		transform->translate(distance);

		lastSave = std::max(0.0f, lastSave - Time::delta());
	}

	void collide(objectTypes oppositeType)
	{
		if (oppositeType == objectTypes::PLAYER && lastSave < 0.1f) {
			lastSave = 2.0f;
			if (hasSound)	getComponent<SoundPlayer>()->play();
		}
	}
};