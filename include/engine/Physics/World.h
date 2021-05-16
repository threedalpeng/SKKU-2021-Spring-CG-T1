#pragma once
#include "btBulletDynamicsCommon.h"

class World
{
public:
	World();
	static void init();
	static void fixedUpdate();

	static btDiscreteDynamicsWorld* world();
private:
	static btDiscreteDynamicsWorld* _dynamicsWorld;
};
