#pragma once

// Standard Library
#include <iostream>

// Engine
#include "engine/Core.h"

// My Application
#include "EndingGUIScript.h"
#include "imgui/imgui.h"

#include "../Tool/MeshMaker.h"

class EndingScene : public Scene {
public:
	EndingScene() : Scene() {};

	void init() {
		GameObject* gui = GameObject::create("Ending GUI");

		addObject(gui);

		/* Initialize Objects with Components */
		SoundPlayer* soundPlayer;

		EndingGUIScript* guiScript = new EndingGUIScript();
		gui->addComponent<ScriptLoader>()->addScript(guiScript);
		soundPlayer = gui->addComponent<SoundPlayer>();
	}
};