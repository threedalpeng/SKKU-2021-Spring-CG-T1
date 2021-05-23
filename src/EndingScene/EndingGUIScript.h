#pragma once
#include "engine/Core.h"
#include "../GameScene/Script/StageGUIScript.h"
#include "../Manager/GameManager.h"
#include "../Manager/ResourceManager.h"
#include <iostream>

#define IM_CLAMP(V, MN, MX)     ((V) < (MN) ? (MN) : (V) > (MX) ? (MX) : (V))

class EndingGUIScript : public Script
{
public:
	EndingGUIScript() : Script() {}
	~EndingGUIScript() {
		EventManager<GuiEvent>::removeListener(guiEventId);
		EventManager<HpChangedEvent>::removeListener(hpEventId);
	}

	enum class Mode {
		DIALOG = 0,
		CREDIT = 1,
	};
	Mode currentMode = Mode::DIALOG;

private:
	int hp = 100;
	int maxHp = 100;

	bool gameFirstStarted = true;

	SoundPlayer* soundPlayer;

	size_t dialogIndex = 0;

	std::vector<std::pair<std::string, std::string>> dialog = {
		{"Partner", "Oh, It's been 12 hours after he called me."},
		{"Partner", "Time's up while watching 'Coin Game'.\nI hope he's okay."},
		{" ", "( As soon as the partner find SpaceWalkie-Talkie... )"},
		{"Player", "I'm back!!!"},
		{"Partner", "Uh... How did you get here? Who helped you?"},
		{" ", "(Explain everything.)"},
		{"Partner", "So, you swim here? Through the asteroids?"},
		{"Player", "Yes. As you told."},
		{"Partner", "And destroy meteor?"},
		{"Player", "Right."},
		{"Partner", "Even defeat aliens?"},
		{"Player", "Sure."},
		{"Partner", "I cannot belive it. You're kidding."},
		{"Player", "Well, I realized that swimming in space is quite useful."},
		{"Player", "I'm think I'm gonna be a space-first space swimmer."},
		{" ", "( Believe or not, over time, space swimming becomes a sport\n that everyone enjoys! )"},
		{" ", "The End."},
	};

	std::vector <std::vector<std::string>> credit = {
		{"- Game Design", "Scenario by Kim Jeong-won", "Game Features by Jaesung Park"},
		{"- Programming", "Kim Jeong-won (System, Audio, Animation...)", "Jaesung Park (System, Physics, Shadow, Particle, AI...)"},
		{"- Art", "2D Images by Kim Jeong-won", "3D Modeling by Jaesung Park" },
		{"- Sound", "Original SoundTrack by Kim Jeong-won"},
		{"Thank You for Playing!"},
	};

	std::vector<std::string> soundList = {
		"sounds/OST/5 - Get some rest.mp3",
	};

	std::vector<std::string> imagePaths = {
		"images/ending1.png",
		"images/ending2.png",
		"images/ending3.png",
	};
	std::vector<GLuint> images = {};
	std::vector<uint> imageIndex = {
		0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2,
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
		soundPlayer->setVolume(0.4f);

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
		case Mode::DIALOG:
			if (Input::getKeyDown(GLFW_KEY_ENTER)) {
				dialogIndex++;
			}
			break;
		case Mode::CREDIT:
			if (Input::getKeyDown(GLFW_KEY_ENTER)) {
				dialogIndex++;
			}
			soundPlayer->play();
			break;
		}

		switch (currentMode) {
		case Mode::DIALOG:
			showDialog();
			break;
		case Mode::CREDIT:
			showCredit();
			break;
		}
	}

private:

	void showDialog() {
		if (dialogIndex < dialog.size()) {
			ImGuiWindowFlags windowFlags = 0;
			windowFlags = windowFlags
				| ImGuiWindowFlags_NoTitleBar
				| ImGuiWindowFlags_NoScrollbar
				| ImGuiWindowFlags_NoScrollWithMouse
				| ImGuiWindowFlags_NoResize
				| ImGuiWindowFlags_NoCollapse
				//| ImGuiWindowFlags_NoBackground
				;

			float minDialogHeight = 150.f;
			float dialogHeight = std::max(minDialogHeight, static_cast<float>(Screen::height()) * 0.35f);

			uint imageToShow = imageIndex[dialogIndex];
			if (imageToShow != 99) {
				ImGui::SetNextWindowPos(
					ImVec2(0, 0),
					ImGuiCond_Always);
				ImGui::SetNextWindowSize(
					ImVec2(static_cast<float>(Screen::width()), static_cast<float>(Screen::height()) - dialogHeight),
					ImGuiCond_Always);

				ImGui::Begin("Image", NULL, windowFlags);
				{
					ImVec2 windowSize = ImGui::GetWindowSize();
					ImGui::Spacing();
					ImGui::SameLine(windowSize.x / 2.f - ((windowSize.y - 10.f) / 2.f));
					ImGui::Image((void*)(intptr_t)images[imageIndex[dialogIndex]], ImVec2(windowSize.y - 10.f, windowSize.y - 10.f));
					if (ImGui::IsItemClicked()) {
						dialogIndex++;
					}
				}
				ImGui::End();
			}

			windowFlags = 0;
			windowFlags = windowFlags
				//| ImGuiWindowFlags_NoTitleBar
				| ImGuiWindowFlags_NoScrollbar
				| ImGuiWindowFlags_NoScrollWithMouse
				| ImGuiWindowFlags_NoResize
				| ImGuiWindowFlags_NoCollapse
				//| ImGuiWindowFlags_NoBackground
				;

			ImGui::SetNextWindowPos(
				ImVec2(0, static_cast<float>(Screen::height()) - dialogHeight),
				ImGuiCond_Always);
			ImGui::SetNextWindowSize(
				ImVec2(static_cast<float>(Screen::width()), dialogHeight),
				ImGuiCond_Always
			);

			ImGui::Begin(dialog[dialogIndex].first.c_str(), NULL, windowFlags);
			{
				ImVec2 windowSize = ImGui::GetWindowSize();

				ImGui::BeginChild("Dialog Item", windowSize);
				{
					ImGui::PushFont(ResourceManager::getFont("consola 20"));

					ImGui::Text(dialog[dialogIndex].second.c_str());

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
			dialogIndex = 0;
			currentMode = Mode::CREDIT;
		}
	}

	void showCredit() {
		ImGuiWindowFlags windowFlags = 0;
		windowFlags = windowFlags
			| ImGuiWindowFlags_NoTitleBar
			| ImGuiWindowFlags_NoScrollbar
			| ImGuiWindowFlags_NoScrollWithMouse
			| ImGuiWindowFlags_NoResize
			| ImGuiWindowFlags_NoCollapse
			| ImGuiWindowFlags_NoBackground
			;

		ImGui::SetNextWindowPos(
			ImVec2(0.f, 0.f),
			ImGuiCond_Always);
		ImGui::SetNextWindowSize(
			ImVec2(static_cast<float>(Screen::width()), static_cast<float>(Screen::height())),
			ImGuiCond_Always
		);

		if (dialogIndex < credit.size()) {
			ImGui::Begin("Credit", NULL, windowFlags);
			{
				ImVec2 windowSize = ImGui::GetWindowSize();

				ImGui::BeginChild("Dialog Item", windowSize);
				{
					ImGui::PushFont(ResourceManager::getFont("consola 20"));
					std::string creditStr = "";
					int maxWidth = 0;
					for (size_t i = 0; i < credit[dialogIndex].size(); i++) {
						creditStr += credit[dialogIndex][i];
						maxWidth = std::max(maxWidth, int(credit[dialogIndex][i].size()));
						if (i < credit[dialogIndex].size() - 1) {
							creditStr += "\n\n";
						}
					}
					int spaces = int(credit[dialogIndex].size() * 2 - 1);
					ImGui::Dummy(ImVec2(0.f, windowSize.y / 2.f - float(spaces + 1) * 10.f));
					ImGui::Spacing();
					ImGui::SameLine(windowSize.x / 2.f - float(maxWidth) * 5.f);
					ImGui::Text(creditStr.c_str());
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
			GameManager::setStage(0);
			GameManager::setChanged(true);
		}
	}
};