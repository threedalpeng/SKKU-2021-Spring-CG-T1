#pragma once

#include "engine/Core.h"
#include "EventCheckerSphere.h"
#include "PlayerScript.h"

class Stage2PlayerScript : public PlayerScript
{
public:
	Stage2PlayerScript() : PlayerScript() {}
	~Stage2PlayerScript() {
		EventManager<GuiEvent>::removeListener(guiEventId);
	}

private:
	int eventProgress = 0;
	int saveEventProgress = 0;
	std::vector<EventCheckerSphere> eventCheckers = {
		EventCheckerSphere(vec3(-278.1f, -2.8f, 0.f), 22.5f, [this]() {
			EventManager<MeteorMoveEvent>::triggerEvent({
				vec3(-246.7f, -47.5f, 0.f),
				vec2(40.f, 20.f),
				5,
				vec3(-1.f, 1.f, 0.f),
				5.f,
				false,
				true
				});
			}),
		EventCheckerSphere(vec3(-216.9f, -48.4f, 0.f), 25.f, [this]() {
			EventManager<MeteorMoveEvent>::triggerEvent({
				vec3(-182.5f, -64.2f, 0.f),
				vec2(40.f, 40.f),
				10,
				vec3(-1.f, 0.f, 0.f),
				8.f,
				false,
				true
				});
			}),
		EventCheckerSphere(vec3(-227.7f, 7.8f, 0.f), 15.f, [this]() {
			EventManager<MeteorMoveEvent>::triggerEvent({
				vec3(-193.8f, 12.8f, 0.f),
				vec2(40.f, 20.f),
				8,
				vec3(-1.f, 0.f, 0.f),
				3.f,
				false,
				true
				});
			}),
		EventCheckerSphere(vec3(-226.5f, 51.7f, 0.f), 15.f, [this]() {
			EventManager<MeteorMoveEvent>::triggerEvent({
				vec3(-193.8f, 42.1f, 0.f),
				vec2(40.f, 20.f),
				8,
				vec3(-1.f, 0.f, 0.f),
				3.f,
				false,
				true
				});
			}),
		EventCheckerSphere(vec3(-150.2f, 8.1f, 0.f), 20.f, [this]() {
			EventManager<MeteorMoveEvent>::triggerEvent({
				vec3(-106.5f, -32.7f, 0.f),
				vec2(20.f, 20.f),
				8,
				vec3(-5.f, 4.f, 0.f),
				5.f,
				false,
				true
				});
			}),
		EventCheckerSphere(vec3(0.f, 21.1f, 0.f), 11.f, [this]() {
			stopPlayer();
			EventManager<GuiEvent>::triggerEvent({ 6 });
			eventProgress = 6;
			saveEventProgress = 6;
			}),
		EventCheckerSphere(vec3(66.f, -14.6f, 0.0f), 22.5f, [this]() {
			EventManager<MeteorMoveEvent>::triggerEvent({
				vec3(20.7f, 5.9f, 0.f),
				vec2(30.f, 20.f),
				4,
				vec3(1.f, -1.f, 0.f),
				9.f,
				false,
				true
				});
			EventManager<MeteorMoveEvent>::triggerEvent({
				vec3(81.0f, -47.7f, 0.f),
				vec2(30.f, 20.f),
				4,
				vec3(-1.f, 1.f, 0.f),
				2.f,
				false,
				true
				});
			}),
		EventCheckerSphere(vec3(123.5f, -21.1f, 0.0f), 10.f, [this]() {
			EventManager<MeteorMoveEvent>::triggerEvent({
				vec3(81.0f, -47.7f, 0.f),
				vec2(30.f, 20.f),
				4,
				vec3(1.f, 1.f, 0.f),
				9.f,
				false,
				true
				});
			EventManager<MeteorMoveEvent>::triggerEvent({
				vec3(161.4f, 16.4f, 0.f),
				vec2(30.f, 30.f),
				4,
				vec3(-1.f, -1.f, 0.f),
				4.f,
				false,
				true
				});
			}),
		EventCheckerSphere(vec3(337.f, 0.f, 0.f), 7.5f, [this]() {
			stopPlayer();
			EventManager<GuiEvent>::triggerEvent({ 7 });
			}),
	};

	uint guiEventId = 0;

public:

	bool guiFinished(const GuiEvent& e) {
		if (e.guiId == -1) // if Enter to Game Mode
			gameStopped = false;
		return true;
	}

	void init() override {
		PlayerScript::init();
		stopPlayer();
		savePoint = btVector3(-330.7f, 0.f, 0.f);
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
			eventCheckers[i].trigger(transform->worldPosition);
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