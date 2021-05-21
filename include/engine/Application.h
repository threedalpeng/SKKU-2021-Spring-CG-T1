#pragma once
#include <vector>

#include "cg/cgut.h"
#include "cg/cgmath.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "engine/Input.h"
#include "engine/Scene/SceneManager.h"
#include "engine/Component/ComponentManager.h"
#include "engine/Graphics/Shader.h"

class Application {
public:
	Application(std::string title);

	// run application
	virtual void run();

protected:
	std::string _title;

	// window objects
	GLFWwindow* _window = nullptr;
	ivec2 _windowSize = cg_default_window_size();

	uint _frame_count = 0;
	float _frame_rate = 100;

	GLuint depthMapFBO;

	// Managers
	ComponentManager _componentManager;

	// Application Lifecycle
	virtual void init();
	virtual void onSceneLoaded();
	virtual void fixedUpdate();
	virtual void update();
	virtual void preRender();
	virtual void render();
	virtual void onGUIRender();
	virtual void terminate();

	// Event Callback Functions
	virtual void reshape(GLFWwindow* window, int width, int height);
	virtual void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods);
	virtual void mouse(GLFWwindow* window, int button, int action, int mods);
	virtual void motion(GLFWwindow* window, double x, double y);
};