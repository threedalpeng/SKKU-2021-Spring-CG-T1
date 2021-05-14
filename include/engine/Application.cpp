#include <iostream>

#include "engine/Application.h"
#include "engine/Screen.h"
#include "engine/Time.h"
#include "engine/Graphics/Camera.h"
#include "engine/Graphics/Light.h"
#include "engine/Graphics/MeshRenderer.h"
#include "engine/Graphics/TextRenderer.h"
#include "engine/Script/ScriptLoader.h"
#include "engine/Transform/Transform.h"

void iterateTransform(GameObject* obj) {
	Transform* transform = obj->getComponent<Transform>();
	transform->update();
	auto children = obj->getChildren();
	if (children.empty()) return;
	for (auto child : children) {
		iterateTransform(child.get());
	}
}

Application::Application(std::string title)
	: _title(title) {}

void Application::run()
{
	init();
	Time::setFixedUpdateRate(_frame_rate);

	for (_frame_count = 0; !glfwWindowShouldClose(_window); _frame_count++) {
		if (SceneManager::sceneLoaded) {
			SceneManager::startScene();
			onSceneLoaded();
			Time::init();
		}
		Time::updateDelta();

		glfwPollEvents();
		Input::poll();

		// fixed time update
		while (Time::needsFixedUpdate()) {
			fixedUpdate();
		}

		if (_frame_count % 20 == 0) {
			printf("                           \r");
			printf(" FPS: %f\r", 1 / Time::delta());
		}
		update();
		render();
	}

	terminate();
}

void Application::init()
{
	if (!(_window = Screen::createWindow(_title, _windowSize))) {
		glfwTerminate();
		exit(1);
	}

	if (!cg_init_extensions(_window)) {
		glfwTerminate();
		exit(1);
	}

	glfwSetWindowUserPointer(_window, this);
	glfwSetWindowSizeCallback(_window, [](GLFWwindow* window, int w, int h) {
		static_cast<Application*>(glfwGetWindowUserPointer(window))->reshape(window, w, h);
		});
	glfwSetKeyCallback(_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		static_cast<Application*>(glfwGetWindowUserPointer(window))->keyboard(window, key, scancode, action, mods);
		});
	glfwSetMouseButtonCallback(_window, [](GLFWwindow* window, int button, int action, int mods) {
		static_cast<Application*>(glfwGetWindowUserPointer(window))->mouse(window, button, action, mods);
		});
	glfwSetCursorPosCallback(_window, [](GLFWwindow* window, double x, double y) {
		static_cast<Application*>(glfwGetWindowUserPointer(window))->motion(window, x, y);
		});

	ServiceLocator::provide<ComponentManager>(&_componentManager);
	SceneManager::init();
}

void Application::onSceneLoaded()
{
	for (auto rootObj : SceneManager::scene()->getRootObjects()) {
		iterateTransform(rootObj);
	}

	if (auto componentList = _componentManager.getComponentList<ScriptLoader>()) {
		for (auto componentPair : *componentList) {
			ScriptLoader* scriptLoader = componentPair.second.get();
			for (auto script : scriptLoader->getScripts()) {
				script->init();
			}
		}
	}

	for (auto rootObj : SceneManager::scene()->getRootObjects()) {
		iterateTransform(rootObj);
	}

	if (auto componentList = _componentManager.getComponentList<Camera>()) {
		for (auto componentPair : *componentList) {
			Camera* camera = componentPair.second.get();
			camera->update();
		}
	}

	SceneManager::sceneLoaded = false;
}

void Application::fixedUpdate()
{
}

void Application::update()
{
	if (auto componentList = _componentManager.getComponentList<ScriptLoader>()) {
		for (auto componentPair : *componentList) {
			ScriptLoader* scriptLoader = componentPair.second.get();
			for (auto script : scriptLoader->getScripts()) {
				script->update();
			}
		}
	}

	for (auto rootObj : SceneManager::scene()->getRootObjects()) {
		iterateTransform(rootObj);
	}

	if (auto componentList = _componentManager.getComponentList<Camera>()) {
		for (auto componentPair : *componentList) {
			Camera* camera = componentPair.second.get();
			camera->update();
		}
	}
}

void Application::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Camera* mainCamera = Camera::main;
	if (!mainCamera) {
		std::cout << "No Camera." << std::endl;
		return;
	}
	mainCamera->render();

	if (auto componentList = _componentManager.getComponentList<Light>()) {
		for (auto componentPair : *componentList) {
			Light* light = componentPair.second.get();
			light->render();
		}
	}

	if (auto componentList = _componentManager.getComponentList<MeshRenderer>()) {
		for (auto componentPair : *componentList) {
			MeshRenderer* renderer = componentPair.second.get();
			renderer->render();
		}
	}

	if (auto componentList = _componentManager.getComponentList<TextRenderer>()) {
		for (auto componentPair : *componentList) {
			TextRenderer* renderer = componentPair.second.get();
			renderer->render();
		}
	}

	glfwSwapBuffers(_window);
}

void Application::onGUIRender()
{
}

void Application::terminate()
{
	cg_destroy_window(_window);
}

void Application::reshape(GLFWwindow* window, int width, int height)
{	
	int w = width;
	int h = height;
	if(width / 16 > height / 9)	w = height / 9 * 16;
	else						h = width / 16 * 9;
	int window_x = (int)((width - w)/2);
	int window_y = (int)((height - h)/2);

	glViewport(window_x, window_y, w, h);
	Screen::processWindowResizeEvent(w, h);
}

void Application::keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Input::processKeyEvent(key, scancode, action, mods);
}

void Application::mouse(GLFWwindow* window, int button, int action, int mods)
{
	Input::processMouseClickEvent(button, action, mods);
}

void Application::motion(GLFWwindow* window, double x, double y)
{
	dvec2 pos;
	glfwGetCursorPos(window, &pos.x, &pos.y);
	Input::processMouseMoveEvent(pos);
}