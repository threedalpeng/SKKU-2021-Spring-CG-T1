#pragma once

// Standard Library
#include <iostream>

// Engine
#include "engine/Core.h"

// My Application
#include "PrologueGUIScript.h"
#include "imgui/imgui.h"

#include "../Tool/MeshMaker.h"

class PrologueScene : public Scene {
public:
	PrologueScene() : Scene() {};

	void init() {
		GameObject* gui = GameObject::create("Prologue GUI");

		addObject(gui);

		/* Initialize Objects with Components */
		SoundPlayer* soundPlayer;

		PrologueGUIScript* guiScript = new PrologueGUIScript();
		gui->addComponent<ScriptLoader>()->addScript(guiScript);
		soundPlayer = gui->addComponent<SoundPlayer>();
	}
};