#include "RigidBody.h"
#include "btCustomRigidBody.h"
#include "World.h"
#include "engine/Time.h"
#include <iostream>

RigidBody::RigidBody(std::shared_ptr<GameObject> obj) : Component(obj) {}

void RigidBody::fixedUpdate() {
	btTransform trans;
	if (_rigidBody && _rigidBody->getMotionState())
	{
		_rigidBody->getMotionState()->getWorldTransform(trans);
	}
	getComponent<Transform>()->setByBtTransform(trans);
}

void RigidBody::loadCollider(Collider* collider) {
	if (collider->mass() != 0.f) {
		collider->calculateLocalInertia();
	}

	btTransform bttransform = getComponent<Transform>()->toBtTransform();

	btDefaultMotionState* motionState = new btDefaultMotionState(bttransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(
		collider->mass(),
		motionState,
		collider->collisionShape(),
		collider->localInertia()
	);

	_rigidBody = new btCustomRigidBody(rbInfo, this);

	World::world()->addRigidBody(_rigidBody);
}

btRigidBody* RigidBody::rigidBody() {
	return _rigidBody;
}

void RigidBody::syncToTransform() {
	Transform* transform = getComponent<Transform>();
	btTransform bttrans = _rigidBody->getWorldTransform();
	btVector3 v = bttrans.getOrigin();
	btQuaternion q = bttrans.getRotation();
	transform->setByBtTransform(bttrans);
}
void RigidBody::syncFromTransform() {
	Transform* transform = getComponent<Transform>();
	btTransform bttrans = transform->toBtTransform();

	_rigidBody->setWorldTransform(bttrans);
}