#pragma once
#include <string>
#include <unordered_set>
#include "engine/Object/GameObject.h"

class Scene {
public:
	Scene() {}
	virtual ~Scene() {
		clear();
	}
	virtual void init();
	virtual void clear();

	void addObject(GameObject* obj) {
		_rootObjects.insert(obj);
	}
	std::unordered_set<GameObject*>& getRootObjects() {
		return _rootObjects;
	}

private:
	std::unordered_set<GameObject*> _rootObjects;
};
