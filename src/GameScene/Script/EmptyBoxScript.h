 #pragma once
#include "engine/Core.h"
#include <iostream>

#include "../Tool/ParticleMaker.h"

class EmptyBoxScript : public Script
{
public:
	EmptyBoxScript() : Script() {}
	EmptyBoxScript(vec3 velocity) : Script() { _velocity = velocity; }
	vec3 _velocity = vec3(0, 0, 0);
	bool hasSound = false;

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
	}
};