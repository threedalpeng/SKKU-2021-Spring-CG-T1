#pragma once
#include "engine/Core.h"
#include "../../Manager/GameManager.h"
#include <iostream>

class GameStartGUIScript : public Script
{
public:
	GameStartGUIScript() : Script() {}

private:
	Transform* transform = nullptr;
	bool clickInBox = false;

public:

	void init() override {
		transform = getComponent<Transform>();
	}

	void update() override {
		/*
		dvec2 mousePoint = Input::getCursorPoint();

		vec3 topright = Camera::main->worldToScreenPoint(transform->localToWorldPoint(vec3(1.f, 1.f, 0.f)));
		vec3 bottomleft = Camera::main->worldToScreenPoint(transform->localToWorldPoint(vec3(0.f, 0.f, 0.0f)));
		text->getComponent<TextRenderer>()->fitToRectangle(
			vec2(topright.x, topright.y),
			vec2(bottomleft.x, bottomleft.y));

		if (Input::getMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT)) {
			if (bottomleft.x <= mousePoint.x && mousePoint.x <= topright.x && topright.y <= mousePoint.y && mousePoint.y <= bottomleft.y)
			{
				clickInBox = true;
			}
			else {
				clickInBox = false;
			}
		}
		if (Input::getMouseButtonUp(GLFW_MOUSE_BUTTON_LEFT)) {
			if (bottomleft.x <= mousePoint.x && mousePoint.x <= topright.x && topright.y <= mousePoint.y && mousePoint.y <= bottomleft.y && clickInBox)
			{
				GameManager::setStage(1);
				GameManager::setChanged(true);
				clickInBox = false;
			}
		}
		*/
		/*
		if (Input::getMouseButton(GLFW_MOUSE_BUTTON_LEFT))
		{
			if (bottomleft.x <= mousePoint.x && mousePoint.x <= topright.x && topright.y <= mousePoint.y && mousePoint.y <= bottomleft.y && clickInBox)
			{
				getComponent<UIRenderer>()->color = vec4(0.4f, 0.4f, 0.8f, 1.0f);
			}
			else {
				getComponent<UIRenderer>()->color = vec4(0.2f, 0.2f, 0.8f, 1.0f);
			}
		}
		else
		{
			getComponent<UIRenderer>()->color = vec4(0.2f, 0.2f, 0.8f, 1.0f);
		}
		*/
	}

	void onGUIRender() override {
		ImGuiWindowFlags windowFlags = 0;
		windowFlags =
			ImGuiWindowFlags_NoTitleBar
			| ImGuiWindowFlags_NoScrollbar
			| ImGuiWindowFlags_NoResize
			| ImGuiWindowFlags_NoBackground;

		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
		// here we set the calculated width and also make the height to be
		// be the height of the main window also with some margin
		ImGui::SetNextWindowSize(
			ImVec2(static_cast<float>(Screen::width()), static_cast<float>(Screen::height())),
			ImGuiCond_Always
		);

		ImGui::SetNextWindowBgAlpha(0.7f);

		ImGui::Begin("Game Start", NULL, windowFlags);

		{
			ImGui::BeginGroup();

			ImVec2 windowSize = ImGui::GetWindowSize();

			ImGui::BeginChild("Title Text", ImVec2(0, windowSize.y - 3 * std::max(minHeight, float(windowSize.y) / 6)));
			ImGui::Text("Space Game");
			ImGui::EndChild();

			ImGui::Separator(); std::string text = "Game Start";

			if (drawButton(text + " ## 1")) {
				std::cout << "Game Start!" << std::endl;
				GameManager::setStage(1);
				GameManager::setChanged(true);
			}
			if (ImGui::IsItemHovered()) {
				ImGui::SetTooltip("Game will be started...");
			}

			ImGui::Spacing();

			if (drawButton(text + " ## 2")) {
				std::cout << "Game Start!" << std::endl;
				GameManager::setStage(1);
				GameManager::setChanged(true);
			}

			ImGui::Spacing();

			if (drawButton(text + " ## 3")) {
				std::cout << "Game Start!" << std::endl;
				GameManager::setStage(1);
				GameManager::setChanged(true);
			}

			ImGui::EndGroup();
		}
		ImGui::End();
	}

	float minWidth = 300.f;
	float minHeight = 100.f;
	bool drawButton(const std::string& text) {
		ImVec2 windowSize = ImGui::GetWindowSize();
		ImVec2 buttonSize = ImVec2(
			std::max(minWidth, float(windowSize.x) / 3),
			std::max(minHeight, float(windowSize.y) / 6));
		ImGui::SameLine(
			ImGui::GetWindowSize().x / 2 - buttonSize.x / 2
		);

		return ImGui::Button(text.c_str(), buttonSize);
	}
};