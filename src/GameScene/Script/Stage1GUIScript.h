#pragma once
#include "engine/Core.h"
#include "../../Manager/GameManager.h"
#include "../../Manager/ResourceManager.h"
#include <iostream>
#define IM_CLAMP(V, MN, MX)     ((V) < (MN) ? (MN) : (V) > (MX) ? (MX) : (V))

bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height)
{
	// Load from file
	int image_width = 0;
	int image_height = 0;
	unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
	if (image_data == NULL)
		return false;

	// Create a OpenGL texture identifier
	GLuint image_texture;
	glGenTextures(1, &image_texture);
	glBindTexture(GL_TEXTURE_2D, image_texture);

	// Setup filtering parameters for display
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

	// Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
	stbi_image_free(image_data);

	*out_texture = image_texture;
	*out_width = image_width;
	*out_height = image_height;

	return true;
}

class Stage1GUIScript : public Script
{
public:
	Stage1GUIScript() : Script() {}
	~Stage1GUIScript() {
		EventManager<GuiEvent>::removeListener(guiEventId);
		EventManager<HpChangedEvent>::removeListener(hpEventId);
	}

	enum class Mode {
		DIALOG,
		HELP,
		GAME,
		PAUSE,
		MONOLOG1,
		MONOLOG2,
		MONOLOG3,
	};
	Mode currentMode = Mode::DIALOG;

private:
	int hp = 50;
	int maxHp = 100;

	bool gameFirstStarted = true;

	SoundPlayer* soundPlayer;

	size_t dialogIndex = 0;
	std::vector<std::pair<std::string, std::string>> dialogs = {
		{"How To", "P - Pause Menu\nESC - Quit to Desktop"},
	};

	std::vector<std::pair<std::string, std::string>> monolog1 = {
		{"Player", "Oh, wait. Radioactive material is being detected in the front now."},
		{"Player", "My spaceship would prevent it but...\nI think I should AVOID that GREEN ROCK."},
	};
	std::vector<std::pair<std::string, std::string>> monolog2 = {
		{"Player", "I feel gravity pulling me... UPWARD?"},
		{"Player", "I need to SLOW DOWN as much as I can!"},
	};
	std::vector<std::pair<std::string, std::string>> monolog3 = {
		{"Player", "What? that is... METEOR!!!"},
		{"Player", "What is happending outside?"},
	};

	std::vector<std::string> helpTexts = {
		"Arrows ", " - Move",
		"ESC ", " - Quit to Desktop\n",
		"P ", " - Pause Menu\n",
		"F1 ", " - Help\n",
		"Click or Press F1 ", " to Leave.\n"
	};
	std::vector<std::string> imagePaths = {
		"images/arrow.jpg",
		"images/ESC.jpg",
		"images/P.jpg",
		"images/F1.jpg",
		"images/F1.jpg",
	};
	std::vector<GLuint> images = {};

	std::vector<std::string> soundList = {
		"sounds/2 - Can you swim... in space.mp3",
		"sounds/3 - Space Swim!.mp3",
	};

	uint guiEventId = 0;
	uint hpEventId = 0;

public:

	bool changeGuiState(const GuiEvent& e) {
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
			printf("hp changed: %d\n", e.hp);
			return changeHp(e);
			});
	}

	void update() override {
	}

	void onGUIRender() override {
		if (currentMode == Mode::GAME) {
			if (gameFirstStarted) {
				soundPlayer->loadSoundFrom(soundList[1]);
				soundPlayer->setType(SoundPlayer::Type::Background);
				soundPlayer->setVolume(0.5f);
				gameFirstStarted = false;
			}
			soundPlayer->play();

			if (Input::getKeyDown(GLFW_KEY_P)) {
				std::cout << "Pause.." << std::endl;
				currentMode = Mode::PAUSE;
			}
			if (Input::getKeyDown(GLFW_KEY_F1)) {
				std::cout << "Help" << std::endl;
				currentMode = Mode::HELP;
			}
		}
		else if (currentMode == Mode::HELP) {
			soundPlayer->pause();
			if (Input::getKeyDown(GLFW_KEY_F1)) {
				currentMode = Mode::GAME;
			}
			if (Input::getKeyDown(GLFW_KEY_ENTER)) {
				currentMode = Mode::GAME;
			}
		}
		else if (currentMode == Mode::DIALOG) {
			if (Input::getKeyDown(GLFW_KEY_ENTER)) {
				dialogIndex++;
			}
		}
		else if (currentMode == Mode::PAUSE) {
			soundPlayer->pause();
		}

		switch (currentMode) {
		case Mode::DIALOG:
			showDialog();
			break;
		case Mode::HELP:
			showHelp();
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
			if (0 <= dialogIndex && dialogIndex <= 1) {
				soundPlayer->play();
			}
			else {
				soundPlayer->stop();
			}

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
			currentMode = Mode::HELP;
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
				for (size_t i = 0; i < images.size(); i++) {
					ImGui::Dummy(ImVec2(0.f, 10.f));
					ImGui::Text(helpTexts[2 * i].c_str());
					ImGui::SameLine();
					ImGui::Image((void*)(intptr_t)images[i], ImVec2(30.f, 30.f));
					ImGui::SameLine();
					ImGui::Text(helpTexts[2 * i + 1].c_str());
				}
			}
			ImGui::EndChild();
			if (ImGui::IsItemClicked()) {
				currentMode = Mode::GAME;
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
			ImGui::EndGroup();
		}
		ImGui::End();
	}
};