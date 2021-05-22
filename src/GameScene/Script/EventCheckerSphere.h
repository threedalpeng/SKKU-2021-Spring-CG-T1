#pragma once
#include "engine/Core.h"

class EventCheckerSphere {
public:
	EventCheckerSphere(vec3 pos, float radius, std::function<void(void)> func) : pos(pos), radius(radius), func(func) {}

	bool trigger(vec3 worldPosition) {
		if (over)
			return false;
		over = ((worldPosition - pos).length() < radius);
		if (over) func();
		return over;
	}
	void retry() {
		over = false;
	}
private:
	vec3 pos;
	float radius = 0.f;
	bool over = false;
	std::function<void(void)> func;
};