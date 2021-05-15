#pragma once
#include "engine/Core.h"
#include "../../Manager/GameManager.h"
#include "../../Manager/ResourceManager.h"
#include <iostream>

class GameStartGUIScript : public Script
{
public:
	GameStartGUIScript() : Script() {}

private:
	Transform* transform = nullptr;
	SoundPlayer* soundPlayer = nullptr;

public:

	void init() override {
		transform = getComponent<Transform>();
		soundPlayer = getComponent<SoundPlayer>();
		soundPlayer->play();
	}

	void update() override {
	}

	void onGUIRender() override {
		ImGuiWindowFlags windowFlags = 0;
		windowFlags =
			ImGuiWindowFlags_NoTitleBar
			| ImGuiWindowFlags_NoScrollbar
			| ImGuiWindowFlags_NoScrollWithMouse
			| ImGuiWindowFlags_NoResize
			| ImGuiWindowFlags_NoBackground;

		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
		ImGui::SetNextWindowSize(
			ImVec2(static_cast<float>(Screen::width()), static_cast<float>(Screen::height())),
			ImGuiCond_Always
		);

		ImGui::SetNextWindowBgAlpha(0.7f);

		ImGui::Begin("Game Start", NULL, windowFlags);
		{
			ImGui::BeginGroup();
			{
				ImVec2 windowSize = ImGui::GetWindowSize();

				ImVec2 childSize = ImVec2(0, windowSize.y - 3 * std::max(minHeight, float(windowSize.y) / 6) - ImGui::GetFrameHeightWithSpacing());
				ImGui::BeginChild("Title Text", childSize);
				{
					std::string titleText = "Space Swim";

					ImGui::PushFont(ResourceManager::getFont("consola 60"));
					float titleTextWidth = ImGui::GetFontSize() * titleText.size() / 2;
					ImGui::Dummy(ImVec2(0.f, childSize.y / 2 - ImGui::GetFontSize() / 2));
					ImGui::Spacing();
					ImGui::SameLine(
						ImGui::GetWindowSize().x / 2 - titleTextWidth / 2
					);
					ImGui::Text(titleText.c_str());
					ImGui::PopFont();
				}
				ImGui::EndChild();

				ImGui::Spacing();

				ImGui::PushFont(ResourceManager::getFont("consola 20"));
				std::string text = "Game Start";

				if (drawButton(text + "## 1")) {
					std::cout << "Game Start!" << std::endl;
					soundPlayer->stop();
					GameManager::setStage(1);
					GameManager::setChanged(true);
				}
				if (ImGui::IsItemHovered()) {
					ImGui::SetTooltip("Game will be started...");
				}

				ImGui::Spacing();

				if (drawButton("Play Sound(Test)")) {
					soundPlayer->play();
				}

				ImGui::Spacing();

				if (drawButton("Pause Sound(Test)")) {
					soundPlayer->pause();
				}
				ImGui::PopFont();
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