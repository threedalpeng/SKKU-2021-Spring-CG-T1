#pragma once
#include "engine/Core.h"
#include "../../Manager/GameManager.h"
#include "../../Manager/ResourceManager.h"
#include <iostream>

class Stage1GUIScript : public Script
{
public:
	Stage1GUIScript() : Script() {}

private:
	bool clickInBox = false;

	enum class Mode {
		DIALOG,
		GAME,
		PAUSE
	};

	Mode currentMode = Mode::GAME;

	size_t dialogIndex = 0;
	std::vector<std::pair<std::string, std::string>> dialogs = {
		{"Player1", "Hi"},
		{"Player2", "Hello"},
		{"Player1", "Good Bye"},
	};

public:

	void init() override {
	}

	void update() override {
	}

	void onGUIRender() override {
		switch (currentMode) {
		case Mode::DIALOG:
			showDialog();
			break;
		case Mode::GAME:
			showGameState();
			break;
		case Mode::PAUSE:
			break;
		}
	}

	void showDialog() {
		ImGuiWindowFlags windowFlags = 0;
		windowFlags = windowFlags
			//| ImGuiWindowFlags_NoTitleBar
			| ImGuiWindowFlags_NoScrollbar
			| ImGuiWindowFlags_NoScrollWithMouse
			| ImGuiWindowFlags_NoResize
			| ImGuiWindowFlags_NoCollapse
			//| ImGuiWindowFlags_NoBackground
			;

		float minDialogHeight = 150.f;
		float dialogHeight = std::max(minDialogHeight, static_cast<float>(Screen::height()) * 0.35f);

		ImGui::SetNextWindowPos(
			ImVec2(0, static_cast<float>(Screen::height()) - dialogHeight),
			ImGuiCond_Always);
		ImGui::SetNextWindowSize(
			ImVec2(static_cast<float>(Screen::width()), dialogHeight),
			ImGuiCond_Always
		);

		if (dialogIndex < dialogs.size()) {
			ImGui::Begin(dialogs[dialogIndex].first.c_str(), NULL, windowFlags);
			{
				ImVec2 windowSize = ImGui::GetWindowSize();

				ImGui::BeginChild("Dialog Item", windowSize);
				{
					ImGui::PushFont(ResourceManager::getFont("consola 20"));

					ImGui::Text(dialogs[dialogIndex].second.c_str());

					ImGui::PopFont();
				}
				ImGui::EndChild();
				if (ImGui::IsItemClicked()) {
					dialogIndex++;
				}
			}
			ImGui::End();
		}
		else {
			currentMode = Mode::GAME;
		}
	}

	void showGameState() {
		ImGuiWindowFlags windowFlags = 0;
		windowFlags = windowFlags
			| ImGuiWindowFlags_NoTitleBar
			| ImGuiWindowFlags_NoScrollbar
			| ImGuiWindowFlags_NoScrollWithMouse
			| ImGuiWindowFlags_NoResize
			| ImGuiWindowFlags_NoCollapse
			//| ImGuiWindowFlags_NoBackground
			;

		ImVec2 hpSize = ImVec2(200.f, 30.f);

		ImGui::SetNextWindowPos(ImVec2(Screen::width() - 100.f, 0.f), ImGuiCond_Always);
		ImGui::SetNextWindowSize(hpSize, ImGuiCond_Always);

		ImGui::Begin("HP", NULL, windowFlags);
		{
		}
		ImGui::End();
	}
};