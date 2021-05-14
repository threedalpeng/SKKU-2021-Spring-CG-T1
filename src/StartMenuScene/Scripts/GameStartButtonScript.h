#pragma once
#include "engine/Core.h"
#include "../../Manager/GameManager.h"
#include <iostream>

class GameStartButtonScript : public Script
{
public:
	GameStartButtonScript() : Script() {}

	GameObject* text = nullptr;

private:
	Transform* transform = nullptr;
	bool clickInBox = false;

public:

	void init() override {
		transform = getComponent<Transform>();
	}

	void update() override {
		dvec2 mousePoint = Input::getCursorPoint();

		vec3 topright = Camera::main->worldToScreenPoint(transform->localToWorldPoint(vec3(1.f, 1.f, 0.f)));
		vec3 bottomleft = Camera::main->worldToScreenPoint(transform->localToWorldPoint(vec3(0.f, 0.f, 0.0f)));
		text->getComponent<TextRenderer>()->fitToRectangle(
			vec2(topright.x, topright.y),
			vec2(bottomleft.x, bottomleft.y));
			
		if (Input::getMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT)) {
			
			if (bottomleft.x <= mousePoint.x && mousePoint.x <= topright.x && topright.y <= mousePoint.y && mousePoint.y <= bottomleft.y)
			{

				clickInBox = true;
			}
			else{
				clickInBox = false;
			}
			
		}
		if (Input::getMouseButtonUp(GLFW_MOUSE_BUTTON_LEFT)) {
			if (bottomleft.x <= mousePoint.x && mousePoint.x <= topright.x && topright.y <= mousePoint.y && mousePoint.y <= bottomleft.y && clickInBox)
			{
				GameManager::setStage(1);
				GameManager::setChanged(true);
				clickInBox = false;
			}
		}
		if (Input::getMouseButton(GLFW_MOUSE_BUTTON_LEFT))
		{
			if (bottomleft.x <= mousePoint.x && mousePoint.x <= topright.x && topright.y <= mousePoint.y && mousePoint.y <= bottomleft.y && clickInBox)
			{
				getComponent<MeshRenderer>()->color = vec4(0.4f, 0.4f, 0.8f, 1.0f);
			}
			else {
				getComponent<MeshRenderer>()->color = vec4(0.2f, 0.2f, 0.8f, 1.0f);
			}
		}
		else
		{
			getComponent<MeshRenderer>()->color = vec4(0.2f, 0.2f, 0.8f, 1.0f);
		}
	}
};