#pragma once
#include "engine/Core.h"

class EventCheckerSphere {
public:
	EventCheckerSphere(vec3 pos, float radius) : pos(pos), radius(radius) {}

	bool shouldTrigger(vec3 worldPosition) {
		if (over)
			return false;
		return over = ((worldPosition - pos).length() < radius);
	}
private:
	vec3 pos;
	float radius = 0.f;
	bool over = false;
};