#pragma once
#include "btBulletDynamicsCommon.h"
#include "bullet/BulletDynamics/Featherstone/btMultiBodyDynamicsWorld.h"

class World
{
public:
	World();
	static void init();
	static void fixedUpdate();
	static void clear();

	static btMultiBodyDynamicsWorld* world();
private:
	static btMultiBodyDynamicsWorld* _dynamicsWorld;
};
