#include "btCustomRigidBody.h"
#include "RigidBody.h"

btCustomRigidBody::btCustomRigidBody(btRigidBody::btRigidBodyConstructionInfo rbInfo, RigidBody* component) :
	btRigidBody(rbInfo)
{
	_component = component;
}

RigidBody* btCustomRigidBody::getObjectComponent() {
	return _component;
}

GameObject* btCustomRigidBody::getObject() {
	GameObject* obj = _component->getCurrentObject().get();
	return (obj ? obj : nullptr);
}