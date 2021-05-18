#pragma once

// Engine
#include "engine/Core.h"
#include "../Manager/GameManager.h"

struct Particle : public Script{

    float   initLift = 2.0f;
    float   remainLife;
  
    Particle() : Script(), _position(0.0f), _velocity(0.0f), _color(1.0f), remainLife(0.0f) { }
    Particle(vec4 position) :Script(), _position(position), _color(1.0f), remainLife(2.0f) { 
        float v_scale = 3.0f;
        _velocity = vec4(0.0f);
        _velocity.x = GameManager::genRandFloat() * v_scale;
        _velocity.y = GameManager::genRandFloat() * v_scale;
        _velocity.z = GameManager::genRandFloat() * v_scale;   
    }

    void init() override {
		transform = getComponent<Transform>();
	}

    void update()
    {
        // float velocityNorm = _velocity.length2();
        _color.x = 1.0f;
        _color.y = 1.0f - remainLife / initLift;
        _color.z = 0.0f;
        _color.a = remainLife / initLift;
        remainLife -= Time::delta();
        if(remainLife < 0.0f)
        {
            
        }
    }
    
protected:
    vec4    _position, _velocity;
    vec4    _color;
private:
	Transform* transform = nullptr;

};    