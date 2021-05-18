#pragma once

// Engine
#include "engine/Core.h"
#include "../Manager/GameManager.h"

class ParticleScript : public Script{
public:
    ParticleScript() : Script(), _position(0.0f), _velocity(0.0f), remainLife(initLife) { }
    ParticleScript(vec3 position) :Script(), _position(position), remainLife(initLife) { 
        float v_scale = 1.5f;
        _velocity = vec3(0.0f);
        _velocity.x = GameManager::genRandFloat() * v_scale;
        _velocity.y = GameManager::genRandFloat() * v_scale;
        _velocity.z = GameManager::genRandFloat() * v_scale / 4;   
    }

    void init() override {
        if(transform) transform = getComponent<Transform>();
	}

    void update()
    {

        // _color = vec4(0.8f, 1.0f - remainLife / initLife, 0.0f, 1.0f);
        // meshRenderer->color = _color;

        transform->translate(_velocity * Time::delta());

        remainLife -= Time::delta();
        remainLife = std::max<float>(0.0f, remainLife);

        if(remainLife < 0.0f)
        {
            
        }
    }
    
    MeshRenderer* meshRenderer;
    Transform* transform = nullptr;
protected:
    float   initLife = 2.0f;
    float   remainLife;

    vec3    _velocity;
    vec3    _position;
    // vec4    _color;

};    