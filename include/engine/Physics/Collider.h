#pragma once
#include "cg/cgmath.h"
#include "engine/Object/GameObject.h"
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

class Collider : public Component
{
public:
	Collider(std::shared_ptr<GameObject> obj) : Component(obj) {};

	btCollisionShape* collisionShape() {
		return _collisionShape;
	}
	void setCollisionShape(btCollisionShape* collisionShape) {
		_collisionShape = collisionShape;
	}

	btScalar mass() { return _mass; }
	void setMass(float mass) {
		_mass = btScalar(mass);
	}

	btVector3 localInertia() { return _localInertia; }
	void setLocalInertia(vec3 localInertia) {
		_localInertia = btVector3(localInertia.x, localInertia.y, localInertia.z);
	}

	void calculateLocalInertia() {
		_collisionShape->calculateLocalInertia(_mass, _localInertia);
	}

private:
	btCollisionShape* _collisionShape = nullptr;
	btScalar _mass = btScalar(0.f);
	btVector3 _localInertia = btVector3();

	static btAlignedObjectArray<btCollisionShape*> _collisionShapes;
};
