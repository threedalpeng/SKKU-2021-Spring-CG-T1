#include "SceneManager.h"
#include "engine/Physics/World.h"

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
		clearScene();
	}
	_currentScene = _sceneToBeLoaded;
	_currentScene->init();
}

void SceneManager::clearScene() {
	_componentManager->clear();
	GameObject::clear();
	World::clear();
	delete _currentScene;
	_currentScene = nullptr;
}

Scene* SceneManager::_currentScene = nullptr;
Scene* SceneManager::_sceneToBeLoaded = nullptr;
ComponentManager* SceneManager::_componentManager = nullptr;