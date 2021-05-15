#pragma once
#include "engine/Core.h"

class PlayerScript : public Script
{
public:
	PlayerScript() : Script() {}
    vec3 _velocity = vec3(0, 0, 0);

private:
	Transform* transform = nullptr;
    

public:

	void init() override {
		transform = getComponent<Transform>();
	}

	void update() override {
        if(Input::getKey(GLFW_KEY_DOWN))   _velocity.y -= 8.0f * Time::delta();
        else if(Input::getKey(GLFW_KEY_UP))  _velocity.y += 8.0f * Time::delta();
        else if(Input::getKey(GLFW_KEY_LEFT))  _velocity.x -= 8.0f * Time::delta();
        else if(Input::getKey(GLFW_KEY_RIGHT))  _velocity.x += 8.0f * Time::delta();
        
        if(Input::getKeyDown(GLFW_KEY_HOME)) 
        {
            _velocity = vec3(0);
            transform->position = vec3(-3.0f, 0.0f, 0.0f);
        }
        
        vec3 distance = _velocity * Time::delta();
        transform->translate(distance);
	}
};