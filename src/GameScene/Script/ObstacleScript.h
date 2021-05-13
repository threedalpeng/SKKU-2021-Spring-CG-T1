#pragma once
#include "engine/Core.h"

class ObstacleScript : public Script
{
public:
	ObstacleScript() : Script() {}
    ObstacleScript(vec3 velocity) : Script() { _velocity = velocity;}
    vec3 _velocity = vec3(0, 0, 0);

private:
	Transform* transform = nullptr;
    

public:

	void init() override {
		transform = getComponent<Transform>();
	}

	void update() override {
        vec3 distance = _velocity * Time::delta();
        transform->translate(distance);
	}
};