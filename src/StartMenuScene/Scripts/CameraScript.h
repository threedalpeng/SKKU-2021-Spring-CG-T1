#pragma once
#include <iostream>
#include "engine/Core.h"

class CameraScript : public Script
{
public:
	CameraScript() : Script() {}

private:
	Camera* camera = nullptr;

	int i = 0;
	float time = 0.f;
	bool increase = true;
	std::vector<GameObject*> objList = std::vector<GameObject*>();
public:

	void init() override {
		getComponent<Transform>()->translate(vec3(0, 0, 10));
		camera = getComponent<Camera>();
		camera->at = vec3(0, 0, 0);
	}

	void update() override {
		if (increase) {
			time += Time::delta();
			if (time >= 1.f) {
				GameObject* sampleObj = GameObject::create("Sample");
				getObject()->addChildren(sampleObj);
				objList.push_back(sampleObj);
				std::cout << "Childrens: " << getObject()->getChildren().size() << std::endl;
				time -= 1.f;
				i++;
			}
			if (i >= 5) {
				increase = false;
			}
		}
		else {
			time += Time::delta();
			if (time >= 1.f) {
				GameObject* objToRemove = objList.back(); objList.pop_back();
				objToRemove->remove();
				std::cout << "Childrens: " << getObject()->getChildren().size() << std::endl;
				time -= 1.f;
				i--;
			}
			if (i <= 0) {
				increase = true;
			}
		}
	}
};
