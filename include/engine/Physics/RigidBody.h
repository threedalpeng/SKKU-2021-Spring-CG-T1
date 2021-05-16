#pragma once

#include "Collider.h"
#include "engine/Object/GameObject.h"
#include "engine/Transform/Transform.h"

#include "btBulletDynamicsCommon.h"

class RigidBody : public Component
{
public:
	RigidBody(std::shared_ptr<GameObject> obj);

	void fixedUpdate();

	void loadCollider(Collider* collider);

	btRigidBody* rigidBody();

private:
	btRigidBody* _rigidBody = nullptr;
};
