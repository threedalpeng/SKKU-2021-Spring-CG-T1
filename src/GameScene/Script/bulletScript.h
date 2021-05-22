 #pragma once
#include "engine/Core.h"
#include <iostream>

#include "../Tool/ParticleMaker.h"
#include "../../Custom/CustomRigidBody.h"
class BulletScript : public Script
{
public:
	BulletScript() : Script() {}
	BulletScript(vec3 velocity) : Script() { _velocity = velocity; }
	vec3 _velocity = vec3(0, 0, 0);
	bool hasSound = false;
	float mass = 0.00000000000001f;

private:
	Transform* transform = nullptr;
	bool leave = true;
	float remainLife = 30.0f;

public:

	void init() override 
	{
		transform = getComponent<Transform>();
	}

	void update() override 
	{
		vec3 distance = _velocity * Time::delta();
		transform->translate(distance);

		remainLife -= Time::delta();

		if (remainLife < 0.1f || !leave) disappear();
	}

	void collide(objectTypes oppositeType)
	{		
		if (oppositeType != objectTypes::PLAYER && leave)
		{
			leave = false;
			if (hasSound)	getComponent<SoundPlayer>()->play();
		}
	}

	void disappear()
	{
		GameObject* thisObject = getObject();
		btRigidBody* objBody = thisObject->getComponent<Transform>()->body;
		// if(objBody) GameManager::dynamicsWorld->removeCollisionObject(objBody);
		if(objBody)	GameManager::removeBodyList.push_back(objBody);
		GameManager::removeList.push_back(thisObject);
	}
};