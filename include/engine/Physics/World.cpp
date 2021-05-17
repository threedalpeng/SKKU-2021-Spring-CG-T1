#include "World.h"
#include "engine/Time.h"

#include "bullet/BulletDynamics/Featherstone/btMultiBodyDynamicsWorld.h"
#include "bullet/BulletDynamics/Featherstone/btMultiBodyConstraintSolver.h"
#include "bullet/BulletDynamics/Featherstone/btMultiBodyPoint2Point.h"
#include "bullet/BulletDynamics/Featherstone/btMultiBodyLinkCollider.h"

#include "engine/Physics/btCustomRigidBody.h"
#include "engine/Physics/RigidBody.h"

#include <iostream>

enum MyFilterModes
{
	FILTER_GROUPAMASKB_AND_GROUPBMASKA2 = 0,
	FILTER_GROUPAMASKB_OR_GROUPBMASKA2
};

struct MyOverlapFilterCallback2 : public btOverlapFilterCallback
{
	int m_filterMode;

	MyOverlapFilterCallback2()
		: m_filterMode(FILTER_GROUPAMASKB_AND_GROUPBMASKA2)
	{
	}

	virtual ~MyOverlapFilterCallback2()
	{
	}
	// return true when pairs need collision
	virtual bool needBroadphaseCollision(btBroadphaseProxy* proxy0, btBroadphaseProxy* proxy1) const
	{
		if (m_filterMode == FILTER_GROUPAMASKB_AND_GROUPBMASKA2)
		{
			bool collides = (proxy0->m_collisionFilterGroup & proxy1->m_collisionFilterMask) != 0;
			collides = collides && (proxy1->m_collisionFilterGroup & proxy0->m_collisionFilterMask);
			return collides;
		}

		if (m_filterMode == FILTER_GROUPAMASKB_OR_GROUPBMASKA2)
		{
			bool collides = (proxy0->m_collisionFilterGroup & proxy1->m_collisionFilterMask) != 0;
			collides = collides || (proxy1->m_collisionFilterGroup & proxy0->m_collisionFilterMask);
			return collides;
		}
		return false;
	}
};

World::World()
{
}

void World::init() {
	btDefaultCollisionConfiguration* m_collisionConfiguration = new btDefaultCollisionConfiguration();
	MyOverlapFilterCallback2* m_filterCallback = new MyOverlapFilterCallback2();
	btCollisionDispatcher* m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
	btOverlappingPairCache* m_pairCache = new btHashedOverlappingPairCache();
	m_pairCache->setOverlapFilterCallback(m_filterCallback);
	btBroadphaseInterface* m_broadphase = new btDbvtBroadphase(m_pairCache);  //btSimpleBroadphase();
	btMultiBodyConstraintSolver* m_solver = new btMultiBodyConstraintSolver;
	_dynamicsWorld = new btMultiBodyDynamicsWorld(m_dispatcher, m_broadphase, m_solver, m_collisionConfiguration);
	_dynamicsWorld->setGravity(btVector3(0.f, 0.f, 0.f));

	/*
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
	btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
	_dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
	*/
}

void World::fixedUpdate() {
	for (int i = _dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = _dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body) {
			std::cout << "velocity1: " << body->getLinearVelocity().x() << std::endl;
			body->getLinearVelocity();
		}
	}
	_dynamicsWorld->stepSimulation(Time::fixedDelta(), 10);

	for (int i = _dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = _dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body) {
			std::cout << "velocity2: " << body->getLinearVelocity().x() << std::endl;
			body->getLinearVelocity();
		}
	}
	int numManifolds = _dynamicsWorld->getDispatcher()->getNumManifolds();
	for (int i = 0; i < numManifolds; i++)
	{
		btPersistentManifold* contactManifold = _dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
		btCollisionObject* obA = (btCollisionObject*)(contactManifold->getBody0());
		btCollisionObject* obB = (btCollisionObject*)(contactManifold->getBody1());

		GameObject* obj1 = reinterpret_cast<btCustomRigidBody*>(btRigidBody::upcast(obA))->getObject();
		GameObject* obj2 = reinterpret_cast<btCustomRigidBody*>(btRigidBody::upcast(obB))->getObject();
		std::cout << obj1->tag() << " " << obj2->tag() << std::endl;
	}
}

void World::clear() {
	for (int i = _dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = _dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		_dynamicsWorld->removeCollisionObject(obj);
		delete obj;
	}
}

btMultiBodyDynamicsWorld* World::world()
{
	return _dynamicsWorld;
}

btMultiBodyDynamicsWorld* World::_dynamicsWorld = nullptr;