#include "RigidBody.h"
#include "World.h"
#include "engine/Time.h"

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

	_rigidBody = new btRigidBody(rbInfo);

	World::world()->addRigidBody(_rigidBody);
}

btRigidBody* RigidBody::rigidBody() {
	return _rigidBody;
}