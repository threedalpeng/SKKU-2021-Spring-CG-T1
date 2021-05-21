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

		if (!leave){
			GameObject* thisObject = getObject();
			MeshRenderer* thisMeshRenderer = thisObject->getComponent<MeshRenderer>();
			thisMeshRenderer->color.w = 0.0f;
		} 

		if (remainLife < 0.1f)
		{
			GameObject* thisObject = getObject();
			GameManager::removeList.push_back(thisObject);
		}
	}

	void collide(objectTypes oppositeType)
	{		
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
		GameObject* thisObject = getObject();
		btRigidBody* objBody = thisObject->getComponent<Transform>()->body;
		if(objBody) GameManager::dynamicsWorld->removeCollisionObject(objBody);
		objBody = nullptr;
		
		if (hasSound)
			getComponent<SoundPlayer>()->play();
	}
};