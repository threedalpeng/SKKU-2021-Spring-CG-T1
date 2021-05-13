#pragma once
#include "engine/Core.h"
#include <iostream>
class GameStartButtonScript : public Script
{
public:
	GameStartButtonScript() : Script() {}

private:
	Transform* transform = nullptr;
	bool clickInBox = true;

public:

	void init() override {
		transform = getComponent<Transform>();
	}

	void update() override {
		if (Input::getMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT)) {
			/*
			if (mousePoint.x > box_left && mousePoint.x < box_right && mousePoint.y > box_bottom && mousePoint.y < box_up)
			{
				clickInBox = true;
			}
			else{
				clickInBox = false;
			}
			*/
		}
		/*
		if (Input::getMouseButtonUp(GLFW_MOUSE_BUTTON_LEFT)) {
			if (bottomleft.x <= mousePoint.x && mousePoint.x <= topright.x && bottomleft.y <= mousePoint.y && mousePoint.y <= topright.y && clickInBox)
			{
				GameManager::setStage(1);
				GameManager::setChanged(true);
				clickInBox = false;
			}
			else
			{
				clickInBox = false;
			}
		}

		*/
		if (Input::getMouseButton(GLFW_MOUSE_BUTTON_LEFT))
		{
			dvec2 mousePoint = Input::getCursorPoint();
			vec3 topright = transform->localToWorldPoint(vec3(1.f, 1.f, 0.f));
			vec3 bottomleft = transform->localToWorldPoint(vec3(0.f, 0.f, 0.0f));
			topright = Camera::main->worldToScreenPoint(topright);
			bottomleft = Camera::main->worldToScreenPoint(bottomleft);

			if (bottomleft.x <= mousePoint.x && mousePoint.x <= topright.x && topright.y <= mousePoint.y && mousePoint.y <= bottomleft.y && clickInBox)
			{
				getComponent<MeshRenderer>()->color = vec4(0.4f, 0.4f, 0.8f, 1.0f);
			}
		}
		else
		{
			getComponent<MeshRenderer>()->color = vec4(0.2f, 0.2f, 0.8f, 1.0f);
		}
	}
};