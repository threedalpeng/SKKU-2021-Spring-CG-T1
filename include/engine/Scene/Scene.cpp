#include <iostream>
#include "Scene.h"
#include "engine/Script/Script.h"

void removeChildrenOf(GameObject* obj) {
	for (auto child : obj->getChildren()) {
		removeChildrenOf(child.get());
	}
}

void Scene::init()
{
}

void Scene::clear()
{
	for (auto obj : _rootObjects) {
		removeChildrenOf(obj);
	}
}