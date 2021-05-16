#pragma once

//*******************************************************************
// bullet3
#include "bullet/btBulletCollisionCommon.h"
#include "bullet/btBulletDynamicsCommon.h"

enum objectTypes
{
	BACKGROUND = 0,
	PLAYER,
	METEOR,
	ITEM,
};

class CustomRigidBody : public btRigidBody
{
	// protected:
public:
	objectTypes objectType;
	GameObject* gameObject;

	CustomRigidBody(btRigidBody::btRigidBodyConstructionInfo rbInfo) : btRigidBody(rbInfo) {}
	CustomRigidBody(btRigidBody::btRigidBodyConstructionInfo rbInfo, objectTypes objectType) : btRigidBody(rbInfo), objectType(objectType) {}
	~CustomRigidBody();

	void setObjectType();
	void getObjectType();

	void setGameObject();
	void getGameObject();
};

CustomRigidBody::~CustomRigidBody()
{
}
