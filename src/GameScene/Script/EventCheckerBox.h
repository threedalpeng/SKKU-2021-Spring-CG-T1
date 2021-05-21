#pragma once
#include "engine/Core.h"

class EventCheckerBox {
public:
	EventCheckerBox(float top, float bottom, float left, float right) : top(top), bottom(bottom), left(left), right(right) {}

	bool shouldTrigger(vec3 worldPosition) {
		return (bottom <= worldPosition.y && worldPosition.y <= top && left <= worldPosition.x && worldPosition.x <= right);
	}
private:
	float top = 0.f;
	float bottom = 0.f;
	float left = 0.f;
	float right = 0.f;
};