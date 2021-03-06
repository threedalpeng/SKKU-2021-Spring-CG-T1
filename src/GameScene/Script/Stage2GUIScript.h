#pragma once
#include "engine/Core.h"
#include "StageGUIScript.h"
#include "../../Manager/GameManager.h"
#include "../../Manager/ResourceManager.h"
#include <iostream>

class Stage2GUIScript : public Script
{
public:
	Stage2GUIScript() : Script() {}
	~Stage2GUIScript() {
		EventManager<GuiEvent>::removeListener(guiEventId);
		EventManager<HpChangedEvent>::removeListener(hpEventId);
	}

	enum class Mode {
		HELP = 0,
		GAME,
		PAUSE,
		MONOLOG1 = 5,
		MONOLOG2 = 6,
		MONOLOG3 = 7,
	};
	Mode currentMode = Mode::MONOLOG1;

private:
	int hp = 100;
	int maxHp = 100;

	bool gameFirstStarted = true;

	SoundPlayer* soundPlayer;

	size_t dialogIndex = 0;

	std::vector<std::pair<std::string, std::string>> monolog1 = {
		{"Player", "The landscape is completely different from when I came in."},
		{"Player", "Something weird is happening.\nLet's get out of here."},
	};
	std::vector<std::pair<std::string, std::string>> monolog2 = {
		{"Player", "Oh my... Everything is collapsing."},
		{"Player", "If I didn't swim out, my body would be tear apart."},
		{"Player", "Get short rest, and leave quickly."},
	};
	std::vector<std::pair<std::string, std::string>> monolog3 = {
		{"Player", "It seems to be safe now."},
		{"Player", "Maybe I have a talent for swimming.\nI'll make a space swimming as a sport when I get back."},
		{"Player", "Before that, I need some rest."},
		{" ", "(You win! Congratulations!)"},
	};

	std::vector<std::string> helpTexts = {
		"Arrows ", " - Move",
		"ESC ", " - Quit to Desktop",
		"P ", " - Pause Menu",
		"F1 ", " - Help",
		"R ", " - Return To Save Point",
		"Space ", " - Shot a bullet",
		"Click or Press F1 ", " to Leave."
	};
	std::vector<std::string> imagePaths = {
		"images/arrow.jpg",
		"images/ESC.jpg",
		"images/P.jpg",
		"images/F1.jpg",
		"images/R.jpg",
		"images/Space.jpg",
	};
	std::vector<GLuint> images = {};
	std::vector<GLuint> imageIndex = {
		0, 1, 2, 3, 4, 5, 3
	};

	std::vector<std::string> soundList = {
		"sounds/OST/4 - Too Dangerous To Swim.mp3",
	};

	uint guiEventId = 0;
	uint hpEventId = 0;

public:

	bool changeGuiState(const GuiEvent& e) {
		if (e.guiId == -1) return false;
		dialogIndex = 0;
		currentMode = static_cast<Mode>(e.guiId);
		return true;
	}
	bool changeHp(const HpChangedEvent& e) {
		hp = e.hp;
		return true;
	}

	void init() override {
		for (size_t i = 0; i < imagePaths.size(); i++) {
			int imageWidth = 0;
			int imageHeight = 0;
			GLuint texture = 0;
			LoadTextureFromFile(imagePaths[i].c_str(), &texture, &imageWidth, &imageHeight);
			images.push_back(texture);
		}
		soundPlayer = getComponent<SoundPlayer>();
		soundPlayer->loadSoundFrom(soundList[0]);
		soundPlayer->setType(SoundPlayer::Type::Background);
		soundPlayer->setVolume(0.2f);

		guiEventId = EventManager<GuiEvent>::addListener([this](const GuiEvent& e)->bool {
			return changeGuiState(e);
			});
		hpEventId = EventManager<HpChangedEvent>::addListener([this](const HpChangedEvent& e)->bool {
			return changeHp(e);
			});
	}

	void update() override {
	}

	void onGUIRender() override {
		switch (currentMode) {
		case Mode::HELP:
			soundPlayer->pause();
			if (Input::getKeyDown(GLFW_KEY_F1)) {
				currentMode = Mode::GAME;
				EventManager<GuiEvent>::triggerEvent({ -1 });
			}
			if (Input::getKeyDown(GLFW_KEY_ENTER)) {
				currentMode = Mode::GAME;
				EventManager<GuiEvent>::triggerEvent({ -1 });
			}
			break;
		case Mode::GAME:
			if (gameFirstStarted) {
				soundPlayer->loadSoundFrom(soundList[0]);
				soundPlayer->setType(SoundPlayer::Type::Background);
				soundPlayer->setVolume(0.5f);
				gameFirstStarted = false;
			}
			soundPlayer->play();

			if (Input::getKeyDown(GLFW_KEY_P)) {
				currentMode = Mode::PAUSE;
			}
			if (Input::getKeyDown(GLFW_KEY_F1)) {
				currentMode = Mode::HELP;
			}
			break;
		case Mode::PAUSE:
			soundPlayer->pause();
			break;
		case Mode::MONOLOG1:
		case Mode::MONOLOG2:
		case Mode::MONOLOG3:
			if (Input::getKeyDown(GLFW_KEY_ENTER)) {
				dialogIndex++;
			}
			break;
		}

		switch (currentMode) {
		case Mode::HELP:
			showHelp();
			break;
		case Mode::GAME:
			showGameState();
			break;
		case Mode::PAUSE:
			showPauseMenu();
			break;
		case Mode::MONOLOG1:
			showMonolog1();
			break;
		case Mode::MONOLOG2:
			showMonolog2();
			break;
		case Mode::MONOLOG3:
			showMonolog3();
			break;
		}
	}

private:

	void showMonolog1() {
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

		if (dialogIndex < monolog1.size()) {
			ImGui::Begin(monolog1[dialogIndex].first.c_str(), NULL, windowFlags);
			{
				ImVec2 windowSize = ImGui::GetWindowSize();

				ImGui::BeginChild("Dialog Item", windowSize);
				{
					ImGui::PushFont(ResourceManager::getFont("consola 20"));

					ImGui::Text(monolog1[dialogIndex].second.c_str());

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
			currentMode = Mode::HELP;
			EventManager<GuiEvent>::triggerEvent({ -1 });
		}
	}

	void showMonolog2() {
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

		if (dialogIndex < monolog2.size()) {
			ImGui::Begin(monolog2[dialogIndex].first.c_str(), NULL, windowFlags);
			{
				ImVec2 windowSize = ImGui::GetWindowSize();

				ImGui::BeginChild("Dialog Item", windowSize);
				{
					ImGui::PushFont(ResourceManager::getFont("consola 20"));

					ImGui::Text(monolog2[dialogIndex].second.c_str());

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
			EventManager<GuiEvent>::triggerEvent({ -1 });
		}
	}

	void showMonolog3() {
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

		if (dialogIndex < monolog3.size()) {
			ImGui::Begin(monolog3[dialogIndex].first.c_str(), NULL, windowFlags);
			{
				ImVec2 windowSize = ImGui::GetWindowSize();

				ImGui::BeginChild("Dialog Item", windowSize);
				{
					ImGui::PushFont(ResourceManager::getFont("consola 20"));

					ImGui::Text(monolog3[dialogIndex].second.c_str());

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
			soundPlayer->stop();
			GameManager::setStage(9);
			GameManager::setChanged(true);
		}
	}

	void showHelp() {
		ImGuiWindowFlags windowFlags = 0;
		windowFlags = windowFlags
			// | ImGuiWindowFlags_NoTitleBar
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

		ImGui::Begin("Help", NULL, windowFlags);
		{
			ImVec2 windowSize = ImGui::GetWindowSize();

			ImGui::PushFont(ResourceManager::getFont("consola 20"));
			ImGui::BeginChild("Help", windowSize);
			{
				for (size_t i = 0; i < helpTexts.size() / 2; i++) {
					ImGui::Dummy(ImVec2(0.f, 10.f));
					ImGui::Text(helpTexts[2 * i].c_str());
					ImGui::SameLine();
					if (imageIndex[i] == 5) {
						ImGui::Image((void*)(intptr_t)images[imageIndex[i]], ImVec2(90.f, 30.f));
					}
					else {
						ImGui::Image((void*)(intptr_t)images[imageIndex[i]], ImVec2(30.f, 30.f));
					}
					ImGui::SameLine();
					ImGui::Text(helpTexts[2 * i + 1].c_str());
				}
			}
			ImGui::EndChild();
			if (ImGui::IsItemClicked()) {
				currentMode = Mode::GAME;
				EventManager<GuiEvent>::triggerEvent({ -1 });
			}
			ImGui::PopFont();
		}
		ImGui::End();
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
				sprintf(buf, "%d/%d", hp, maxHp);
				ImGui::ProgressBar(float(hp) / float(maxHp), ImVec2(-1.f, -1.f), buf);
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

		ImGui::Begin("Menu", NULL, windowFlags);
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
					EventManager<GuiEvent>::triggerEvent({ -1 });
				}
				ImGui::Spacing();

				ImGui::SameLine(
					pauseMenuWindowSize.x / 2.f - buttonSize.x / 2
				);
				if (ImGui::Button("To Main Title", buttonSize)) {
					GameManager::setStage(0);
					GameManager::setChanged(true);
				}
				ImGui::Spacing();

				ImGui::SameLine(
					pauseMenuWindowSize.x / 2.f - buttonSize.x / 2
				);
				if (ImGui::Button("Quit To Desktop", buttonSize)) {
					Screen::quit();
				};
			}
			ImGui::EndGroup();
		}
		ImGui::End();
	}
};