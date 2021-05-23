#pragma once

#include "engine/Core.h"
#include "EventCheckerSphere.h"
#include "PlayerScript.h"

class Stage1PlayerScript : public PlayerScript
{
public:
	Stage1PlayerScript() : PlayerScript() {}
	~Stage1PlayerScript() {
		EventManager<GuiEvent>::removeListener(guiEventId);
	}

private:
	int eventProgress = 0;
	int saveEventProgress = 0;
	std::vector<EventCheckerSphere> eventCheckers = {
		EventCheckerSphere(vec3(31.19f, -64.36f, 0.0f), 5.f, [this]() {
			stopPlayer();
			EventManager<GuiEvent>::triggerEvent({ 4 }); // Monolog 1
			}),
		EventCheckerSphere(vec3(81.12f, -59.12f, 0.0f), 5.f, [this]() {
			stopPlayer();
			EventManager<GuiEvent>::triggerEvent({ 5 }); // Monolog 2
			gravityOn = true;
			}),
		EventCheckerSphere(vec3(81.12f, 25.7f, 0.0f), 5.f, [this]() {
			GameManager::dynamicsWorld->setGravity(btVector3(0.f, 0.f, 0.f));
			}),
		EventCheckerSphere(vec3(100.6f, 30.2f, 0.f), 8.5f, [this]() {
			stopPlayer();
			saveEventProgress = eventProgress + 1;
			EventManager<GuiEvent>::triggerEvent({ 6 }); // Monolog 3
			}),
		EventCheckerSphere(vec3(247.8f, 35.7f, 0.f), 15.f, [this]() {
			stopPlayer();
			EventManager<GuiEvent>::triggerEvent({ 7 }); // Monolog 4
			}),
		EventCheckerSphere(vec3(324.95f, 35.7f, 0.f), 15.f, [this]() {
			stopPlayer();
			EventManager<GuiEvent>::triggerEvent({ 8 }); // End
			}),
	};

	uint guiEventId = 0;

	bool gravityOn = false;

public:

	bool guiFinished(const GuiEvent& e) {
		if (e.guiId == -1) // if Enter to Game Mode
			gameStopped = false;
		if (gravityOn) {
			GameManager::dynamicsWorld->setGravity(btVector3(0.f, 7.f, 0.f));
			gravityOn = false;
		}
		return true;
	}

	void init() override {
		PlayerScript::init();
		savePoint = btVector3(-3.0f, 0.f, 0.f);
		guiEventId = EventManager<GuiEvent>::addListener([this](const GuiEvent& e) -> bool {
			return guiFinished(e);
			});
	}

	void update() override {
		//printf("%f, %f\n", transform->worldPosition.x, transform->worldPosition.y);
		if (gameStopped) {
			return;
		}
		for (size_t i = eventProgress; i < eventCheckers.size(); i++) {
			if (eventCheckers[i].trigger(transform->worldPosition))
				eventProgress++;
			else break;
		}

		if (Input::getKeyDown(GLFW_KEY_R))
		{
			eventProgress = saveEventProgress;
			for (size_t i = eventProgress; i < eventCheckers.size(); i++) {
				eventCheckers[i].retry();
			}
		}

		PlayerScript::update();
	}
};