#include "SceneManager.h"

void SceneManager::init() {
	_componentManager = ServiceLocator::getService<ComponentManager>();
}

Scene* SceneManager::scene() {
	return _currentScene;
}

bool SceneManager::sceneLoaded = false;
void SceneManager::loadScene(Scene* scene) {
	_sceneToBeLoaded = scene;
	sceneLoaded = true;
}
void SceneManager::startScene() {
	if (_currentScene) {
		delete _currentScene;
		_currentScene = nullptr;
	}
	_currentScene = _sceneToBeLoaded;
	_currentScene->init();
}

Scene* SceneManager::_currentScene = nullptr;
Scene* SceneManager::_sceneToBeLoaded = nullptr;
ComponentManager* SceneManager::_componentManager = nullptr;