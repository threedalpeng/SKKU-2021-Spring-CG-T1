#include <iostream>

#include "engine/Application.h"
#include "engine/Screen.h"
#include "engine/Time.h"
#include "engine/Graphics/Camera.h"
#include "engine/Graphics/Light.h"
#include "engine/Graphics/MeshRenderer.h"
#include "engine/Script/ScriptLoader.h"
#include "engine/Sound/SoundPlayer.h"
#include "engine/Transform/Transform.h"

#include "bullet/src/btBulletCollisionCommon.h"
#include "bullet/src/btBulletDynamicsCommon.h"

void setImGuiStyle(float highDPIscaleFactor)
{
	ImGuiStyle& style = ImGui::GetStyle();

	style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.21f, 0.22f, 0.54f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.40f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.18f, 0.18f, 0.18f, 0.67f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.29f, 0.29f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.44f, 0.44f, 0.44f, 0.40f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.46f, 0.47f, 0.48f, 1.00f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.42f, 0.42f, 0.42f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.70f, 0.70f, 0.70f, 0.31f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.70f, 0.70f, 0.70f, 0.80f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.48f, 0.50f, 0.52f, 1.00f);
	style.Colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.72f, 0.72f, 0.72f, 0.78f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.91f, 0.91f, 0.91f, 0.25f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.81f, 0.81f, 0.81f, 0.67f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.46f, 0.46f, 0.46f, 0.95f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.73f, 0.60f, 0.15f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.87f, 0.87f, 0.87f, 0.35f);
	//style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);

	style.WindowPadding = ImVec2(8, 6);
	style.WindowRounding = 0.0f;
	style.FramePadding = ImVec2(5, 7);
	//style.FrameRounding            = 0.0f;
	style.ItemSpacing = ImVec2(5, 5);
	// style.ItemInnerSpacing         = ImVec2(1, 1);
	// style.TouchExtraPadding        = ImVec2(0, 0);
	// style.IndentSpacing            = 6.0f;
	// style.ScrollbarSize            = 12.0f;
	// style.ScrollbarRounding        = 16.0f;
	// style.GrabMinSize              = 20.0f;
	// style.GrabRounding             = 2.0f;
	// style.WindowTitleAlign.x = 0.50f;
	// style.FrameBorderSize = 0.0f;
	// style.WindowBorderSize = 1.0f;

	//style.ScaleAllSizes(highDPIscaleFactor);
}

void loadingGui() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGuiWindowFlags windowFlags = 0;
	windowFlags = windowFlags
		| ImGuiWindowFlags_NoTitleBar
		| ImGuiWindowFlags_NoScrollbar
		| ImGuiWindowFlags_NoScrollWithMouse
		| ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_NoCollapse
		//| ImGuiWindowFlags_NoBackground
		;

	ImVec2 windowSize = ImVec2(float(Screen::width()), float(Screen::height()));
	ImGui::SetNextWindowPos(ImVec2(0.f, 0.f), ImGuiCond_Always);
	ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);
	ImGui::SetNextWindowBgAlpha(1.f);
	ImGui::Begin("Hi", NULL, windowFlags);
	{
		ImGui::Text("Hi");
	}
	ImGui::End();
	ImGui::SetNextWindowBgAlpha(0.6f);

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

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
			loadingGui();
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

		/*
		if (_frame_count % 20 == 0) {
			printf("                           \r");
			printf(" FPS: %f\r", 1 / Time::delta());
		}
		*/
		update();

		// glClear(GL_DEPTH_BUFFER_BIT);
		preRender();
		render();
		onGUIRender();
		glfwSwapBuffers(_window);
	}

	terminate();
}

void Application::init()
{
	/* OpenGL, GLFW, GLAD */
	if (!(_window = Screen::createWindow(_title, _windowSize))) {
		glfwTerminate();
		exit(1);
	}
	glfwSwapInterval(1);

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

	/* ImGui */
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(_window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

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

	if (auto componentList = _componentManager.getComponentList<Light>()) {
		for (auto componentPair : *componentList) {
			Light* light = componentPair.second.get();
			light->init();
		}
	}

	if (auto componentList = _componentManager.getComponentList<Light>()) {
		for (auto componentPair : *componentList) {
			Light* light = componentPair.second.get();
			light->update();
		}
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
	for (auto rootObj : SceneManager::scene()->getRootObjects()) {
		iterateTransform(rootObj);
	}

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

	if (auto componentList = _componentManager.getComponentList<Light>()) {
		for (auto componentPair : *componentList) {
			Light* light = componentPair.second.get();
			light->update();
		}
	}

	if (auto componentList = _componentManager.getComponentList<Camera>()) {
		for (auto componentPair : *componentList) {
			Camera* camera = componentPair.second.get();
			camera->update();
		}
	}
}

void Application::preRender() {
	if (auto componentList = _componentManager.getComponentList<Light>()) {
		for (auto componentPair : *componentList) {
			Light* light = componentPair.second.get();
			light->renderDepth();
		}
	}

	if (auto componentList = _componentManager.getComponentList<MeshRenderer>()) {
		for (auto componentPair : *componentList) {
			MeshRenderer* renderer = componentPair.second.get();
			renderer->renderDepth();
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// glfwSwapBuffers(_window);
}

void Application::render()
{
	glViewport(0, 0, Screen::width(), Screen::height());
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Camera* mainCamera = Camera::main;
	if (!mainCamera) {
		std::cout << "No Camera." << std::endl;
	}
	else {
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
	}
}

void Application::onGUIRender()
{
	bool show_demo_window = false;
	bool show_another_window = false;

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	if (show_demo_window) { ImGui::ShowDemoWindow(&show_demo_window); }

	if (auto componentList = _componentManager.getComponentList<ScriptLoader>()) {
		for (auto componentPair : *componentList) {
			ScriptLoader* scriptLoader = componentPair.second.get();
			for (auto script : scriptLoader->getScripts()) {
				script->onGUIRender();
			}
		}
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Application::terminate()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	cg_destroy_window(_window);
}

void Application::reshape(GLFWwindow* window, int width, int height)
{
	/*
	int w = width;
	int h = height;
	if(width / 16 > height / 9)	w = height / 9 * 16;
	else						h = width / 16 * 9;
	int window_x = (int)((width - w)/2);
	int window_y = (int)((height - h)/2);
	glViewport(window_x, window_y, w, h);
	Screen::processWindowResizeEvent(w, h);
	*/

	glViewport(0, 0, width, height);
	Screen::processWindowResizeEvent(width, height);
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