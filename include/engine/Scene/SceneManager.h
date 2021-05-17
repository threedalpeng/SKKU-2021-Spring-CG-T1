#pragma once
#include "Scene.h"
#include "engine/ServiceLocator.h"
#include "engine/Component/ComponentManager.h"

class SceneManager
{
public:
	static void init();

	static Scene* scene();

	static bool sceneLoaded;
	static void loadScene(Scene* scene);
	static void clearScene();
	static void startScene();
private:
	static Scene* _currentScene;
	static Scene* _sceneToBeLoaded;
	static ComponentManager* _componentManager;
};
