#pragma once

#include "engine/Object/GameObject.h"
#include "btBulletDynamicsCommon.h"

class RigidBody;

class btCustomRigidBody : public btRigidBody {
public:
	btCustomRigidBody(btRigidBody::btRigidBodyConstructionInfo rbInfo, RigidBody* component);

	RigidBody* getObjectComponent();
	GameObject* getObject();
private:
	RigidBody* _component;
};