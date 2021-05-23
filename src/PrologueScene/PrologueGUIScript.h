#pragma once
#include "engine/Core.h"
#include "../GameScene/Script/StageGUIScript.h"
#include "../Manager/GameManager.h"
#include "../Manager/ResourceManager.h"
#include <iostream>

#define IM_CLAMP(V, MN, MX)     ((V) < (MN) ? (MN) : (V) > (MX) ? (MX) : (V))

class PrologueGUIScript : public Script
{
public:
	PrologueGUIScript() : Script() {}
	~PrologueGUIScript() {
		EventManager<GuiEvent>::removeListener(guiEventId);
		EventManager<HpChangedEvent>::removeListener(hpEventId);
	}

	enum class Mode {
		DIALOG = 0,
	};
	Mode currentMode = Mode::DIALOG;

private:
	int hp = 100;
	int maxHp = 100;

	bool gameFirstStarted = true;

	SoundPlayer* soundPlayer;

	size_t dialogIndex = 0;
	std::vector<std::pair<std::string, std::string>> dialogs = {
		{" ", "Press 'Enter' or 'Click' dialogs to progress story."},
		{" ", "(In the distant future, quite a long time after Elon\nMusk went to Mars...)"},
		{"Player", "Whoo.. there's nowhere else to go further.\nI should go back."},
		{" ", "(At that moment, you hear the engine shut down.)"},
		{"Player", "Uh... The spaceship is running out of fuel.\nI'm gonna be nagged again..."},
		{" ", "(You take your hand to SpaceWalkie-Talkie.)"},
		{"Partner", "What? Ran out of fuel again?"},
		{"Partner", "Get back into space flight school.\nNo, go back to elementary school!"},
		{"Partner", "Who on earth is rushing into space\nwithout checking the fuel?"},
		{"Player", "Okay, I'm sorry. I'll be careful next time.\nI'm getting hungry, so hurry up and pick me up."},
		{"Partner", "You said the same thing last time!\nI'm really not going this time."},
		{"Player", "What? So what am I supposed to do?"},
		{"Partner", "I don't know.\nWhy don't you SWIM all the way here?"},
		{"Player", "Hey, Wait...!"},
		{" ", "(There is no response...)"},
		{"Partner", "(If I scared him little, He'll listen me next time.\nLet's wait for an hour and call him again.)"},
		{"Player", "Hello? Psh, I really don't think he's coming this time."},
		{"Player", "Wait, SWIM?"},
		{"Player", "Well, it's a great idea!"},
		{" ", "(So you decide to swim back to base.)"},
		{" ", "(I don't know how but... Good Luck!)"}
	};

	std::vector<std::string> soundList = {
		"sounds/OST/2 - Can you swim... in space.mp3",
	};

	std::vector<std::string> imagePaths = {
		"images/prologue1.png",
		"images/prologue2.png",
		"images/prologue3.png",
		"images/prologue4.png",
	};
	std::vector<GLuint> images = {};
	std::vector<uint> imageIndex = {
		99, 99, 99, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 2, 2, 3, 3,
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
		}

		switch (currentMode) {
		case Mode::DIALOG:
			if (dialogIndex == 17) {
				soundPlayer->play();
			}
			showDialog();
			break;
		}
	}

private:

	void showDialog() {
		if (dialogIndex < dialogs.size()) {
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
			GameManager::setStage(1);
			GameManager::setChanged(true);
		}
	}
};