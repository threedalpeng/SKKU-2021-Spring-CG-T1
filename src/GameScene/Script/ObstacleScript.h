 #pragma once
#include "engine/Core.h"
#include <iostream>

#include "../Tool/ParticleMaker.h"

class ObstacleScript : public Script
{
public:
	ObstacleScript() : Script() {}
	ObstacleScript(vec3 velocity) : Script() { _velocity = velocity; }
	vec3 _velocity = vec3(0, 0, 0);
	bool hasSound = false;

private:
	Transform* transform = nullptr;
	bool leave = true;
	float remainLife = 2.0f;

public:

	void init() override 
	{
		transform = getComponent<Transform>();
	}

	void update() override 
	{
		vec3 distance = _velocity * Time::delta();
		transform->translate(distance);

		if (!leave){
			remainLife -= Time::delta();
			GameObject* thisObject = getObject();
			MeshRenderer* thisMeshRenderer = thisObject->getComponent<MeshRenderer>();
			thisMeshRenderer->color.w = std::max(remainLife / 2.0f, 0.0f);
		} 

		if (remainLife < 0.0f)
		{
			GameObject* thisObject = getObject();
			GameManager::removeList.push_back(thisObject);
		}
	}

	void collide()
	{
		// getComponent<SoundPlayer>()->play();
		// std::cout << "collide meteor " << getComponent<SoundPlayer>() << std::endl;
		// std::cout << "collide meteor " << soundPlayer << std::endl;

		// soundPlayer->play();
		
		if (leave)
		{
			explode();
			GameObject* thisObject = getObject();
			MeshRenderer* thisMeshRenderer = thisObject->getComponent<MeshRenderer>();
			thisMeshRenderer->isColored = true;
			thisMeshRenderer->isShaded = true;
			thisMeshRenderer->hasTexture = true;
			thisMeshRenderer->color = vec4(1.0f, 1.0f, 1.0f, 1.0f);

			leave = false;
		}
	}

		

	void explode()
	{
		ParticleMaker::makeExplodeParticle(transform->position);
		if (hasSound)
			getComponent<SoundPlayer>()->play();
	}
};