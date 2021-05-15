#pragma once
#include "engine/Core.h"
#include "../../Manager/GameManager.h"
#include "../../Manager/ResourceManager.h"
#include <iostream>
#define IM_CLAMP(V, MN, MX)     ((V) < (MN) ? (MN) : (V) > (MX) ? (MX) : (V))

class Stage1GUIScript : public Script
{
public:
	Stage1GUIScript() : Script() {}

	enum class Mode {
		DIALOG,
		GAME,
		PAUSE
	};
	Mode currentMode = Mode::GAME;

private:
	// Sample data
	float hp = 200;
	float maxHp = 200;

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
		static bool damaged = true;
		if (currentMode == Mode::GAME) {
			if (damaged) {
				hp -= 10.f * Time::delta();
			}
			else {
				hp += 10.f * Time::delta();
			}
			if (hp < 0.0f) {
				hp = 0.0f;
				damaged = false;
			}
			if (hp > maxHp) {
				hp = maxHp;
				damaged = true;
			}

			if (Input::getKeyDown(GLFW_KEY_P)) {
				std::cout << "Pause.." << std::endl;
				currentMode = Mode::PAUSE;
			}
		}
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
			showPauseMenu();
			break;
		}
	}

private:

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

		ImVec2 hpWindowSize = ImVec2(300.f, 30.f);
		ImVec2 hpTextSize = ImVec2(200.f, 30.f);
		ImVec2 hpSize = ImVec2(200.f, 30.f);

		ImGui::SetNextWindowPos(ImVec2(Screen::width() - hpWindowSize.x, 0.f), ImGuiCond_Always);
		ImGui::SetNextWindowSize(hpWindowSize, ImGuiCond_Always);

		ImGui::Begin("HP", NULL, windowFlags);
		{
			ImGui::BeginGroup();
			{
				ImGui::Text("HP: ");
				ImGui::SameLine(40.f);

				char buf[32];
				sprintf(buf, "%d/%d", int(hp), int(maxHp));
				ImGui::ProgressBar(hp / maxHp, ImVec2(-1.f, -1.f), buf);
			}
		}
		ImGui::End();
	}

	void showPauseMenu() {
		ImGuiWindowFlags windowFlags = 0;
		windowFlags = windowFlags
			| ImGuiWindowFlags_NoTitleBar
			| ImGuiWindowFlags_NoScrollbar
			| ImGuiWindowFlags_NoScrollWithMouse
			| ImGuiWindowFlags_NoResize
			| ImGuiWindowFlags_NoCollapse
			//| ImGuiWindowFlags_NoBackground
			;

		float minPauseMenuWindowWidth = 480.f;
		float minPauseMenuWindowHeight = 360.f;
		float scaledHeight = Screen::height() * 0.75f;
		float scaledWidth = float(Screen::height());
		ImVec2 pauseMenuWindowSize = ImVec2(
			std::max(minPauseMenuWindowWidth, scaledWidth),
			std::max(minPauseMenuWindowHeight, scaledHeight));

		ImGui::SetNextWindowPos(ImVec2(
			float(Screen::width()) / 2.f - pauseMenuWindowSize.x / 2.f,
			float(Screen::height()) / 2.f - pauseMenuWindowSize.y / 2.f),
			ImGuiCond_Always);
		ImGui::SetNextWindowSize(pauseMenuWindowSize, ImGuiCond_Always);

		ImGui::Begin("HP", NULL, windowFlags);
		{
			ImGui::BeginGroup();
			{
				std::string menuText = "Menu";
				ImGui::PushFont(ResourceManager::getFont("consola 40"));
				float titleTextWidth = ImGui::GetFontSize() * menuText.size() / 2.f;
				ImGui::Dummy(ImVec2(0.f, scaledHeight * 0.1f));
				ImGui::Spacing();
				ImGui::SameLine(pauseMenuWindowSize.x / 2.f - titleTextWidth / 2.f);
				ImGui::Text("Menu");
				ImGui::SameLine(0.f);
				ImGui::Dummy(ImVec2(0.f, scaledHeight * 0.2f));
				ImGui::PopFont();
				ImGui::Spacing();

				ImVec2 buttonSize = ImVec2(scaledWidth * 0.8f, scaledHeight * 0.2f);

				ImGui::SameLine(
					pauseMenuWindowSize.x / 2.f - buttonSize.x / 2
				);
				if (ImGui::Button("Resume", buttonSize)) {
					currentMode = Mode::GAME;
				}
				ImGui::Spacing();

				ImGui::SameLine(
					pauseMenuWindowSize.x / 2.f - buttonSize.x / 2
				);
				if (ImGui::Button("To Main Title", buttonSize)) {
					// do something
				}
				ImGui::Spacing();

				ImGui::SameLine(
					pauseMenuWindowSize.x / 2.f - buttonSize.x / 2
				);
				if (ImGui::Button("Quit To Desktop", buttonSize)) {
					// do something;
				};
			}
		}
		ImGui::End();
	}
};