#pragma once
#include "engine/Core.h"
#include <iostream>
class GameStartButtonScript : public Script
{
public:
	GameStartButtonScript() : Script() {}

private:
	Transform* transform = nullptr;

public:

	void init() override {
		transform = getComponent<Transform>();
	}

	void update() override {
		if(Input::getMouseButton(GLFW_MOUSE_BUTTON_LEFT) )
		{
			dvec2 mousePoint = Input::getCursorPoint();			
			float wx, mx, x_p, box_left, box_right;

			wx = (float)cg_default_window_size().x;
			mx = 5.0f;	// it shoud be modified !!!

			x_p =  (getComponent<Transform>()->getModelMatrix() * vec4(0.0f, 0, 0, 0.0f)).x;
			box_left = wx / 2.0f * (1.0f + x_p / mx);

			x_p =  (getComponent<Transform>()->getModelMatrix() * vec4(1.0f, 1.0f, 0, 0.0f)).x;
			box_right = wx / 2.0f * (1.0f + x_p / mx);

			float wy, my, y_p, box_bottom, box_up;
			// Camera::worldToViewportPoint();
			wy = (float)cg_default_window_size().y;
			my = 3.0f;	// it shoud be modified !!!

			y_p =  (getComponent<Transform>()->getModelMatrix() * vec4(0.0f, 0, 0, 0.0f)).y;
			box_bottom = wy / 2.0f * (1.0f + y_p / my);

			y_p =  (getComponent<Transform>()->getModelMatrix() * vec4(1.0f, 1.0f, 0, 0.0f)).y;
			box_up = wy / 2.0f * (1.0f + y_p / my);


			if(mousePoint.x > box_left && mousePoint.x < box_right && mousePoint.y > box_bottom && mousePoint.y < box_up)
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