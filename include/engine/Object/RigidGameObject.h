#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <typeindex>

class Component;
class ComponentManager;
class GameObject;
#include "GameObject.h"

//*******************************************************************
// bullet3
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"


class RigidGamePbject : public GameObject, public btRigidBody
{
public:
    RigidGamePbject(std::string name, btRigidBody::btRigidBodyConstructionInfo rbInfo) : GameObject(name), btRigidBody(rbInfo) {}
    virtual ~RigidGamePbject() {}

};