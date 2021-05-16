#include "World.h"

World::World()
{
}

void World::init() {
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();

	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

	btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();

	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

	_dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
}

void World::fixedUpdate()
{
}

btDiscreteDynamicsWorld* World::world()
{
	return _dynamicsWorld;
}

btDiscreteDynamicsWorld* World::_dynamicsWorld = nullptr;