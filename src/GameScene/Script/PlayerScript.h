#pragma once
#include "engine/Core.h"
#include "EventCheckerSphere.h"
//*******************************************************************
// bullet3
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#include "../../Custom/CustomRigidBody.h"

#include <iostream>

class PlayerScript : public Script
{
public:
	PlayerScript() : Script() {}
	~PlayerScript() {
		EventManager<GuiEvent>::removeListener(guiEventId);
	}

	int HP = 100;
	btVector3 savePoint = btVector3(-3.0f, 0, 0);
	float 	lastWallCollistion = 0.0f;

private:
	Transform* transform = nullptr;
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
			savePoint = btVector3(100.6f, 30.2f, 0.f);
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

	bool gameStopped = true;
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
		transform = getComponent<Transform>();
		guiEventId = EventManager<GuiEvent>::addListener([this](const GuiEvent& e) -> bool {
			return guiFinished(e);
			});
	}

	void update() override {
		//printf("%f, %f\n", transform->worldPosition.x, transform->worldPosition.y);
		if (gameStopped) {
			return;
		}
		for (int i = eventProgress; i < eventCheckers.size(); i++) {
			if (eventCheckers[i].trigger(transform->worldPosition))
				eventProgress++;
			else break;
		}

		btVector3 addVelocity = btVector3(0, 0, 0);
		btVector3 currentVelocity = transform->getVelocityBT();
		if (currentVelocity.getX() >= -8.0f && Input::getKey(GLFW_KEY_LEFT))  addVelocity.setX(-8.0f * Time::delta());
		else if (currentVelocity.getX() <= 8.0f && Input::getKey(GLFW_KEY_RIGHT))  addVelocity.setX(+8.0f * Time::delta());
		if (currentVelocity.getY() >= -8.0f && Input::getKey(GLFW_KEY_DOWN))   addVelocity.setY(-8.0f * Time::delta());
		else if (currentVelocity.getY() <= 8.0f && Input::getKey(GLFW_KEY_UP))  addVelocity.setY(+8.0f * Time::delta());

		if (Input::getKeyDown(GLFW_KEY_R))
		{
			// _velocity = vec3(0);
			// transform->position = vec3(-3.0f, 0.0f, 0.0f);
			transform->setVelocityBT(btVector3(0, 0, 0));
			transform->setWorlPositionBT(savePoint);
			eventProgress = saveEventProgress;
			for (int i = eventProgress; i < eventCheckers.size(); i++) {
				eventCheckers[i].retry();
			}
		}
		else if (Input::getKeyDown(GLFW_KEY_SPACE))
		{
		}
		transform->addVelocityBT(addVelocity);
		lastWallCollistion = std::max(0.0f, lastWallCollistion - Time::delta());
	}

	void collide(objectTypes oppositeType) {
		if (oppositeType == objectTypes::METEOR) {
			HP -= 10;
			std::cout << "Now player's HP is " << HP << "\r";
			EventManager<HpChangedEvent>::triggerEvent({ HP });
		}
		else if (oppositeType == objectTypes::WALL && lastWallCollistion < 0.1f) {
			HP -= 1;
			lastWallCollistion = 1.0f;
			std::cout << "Now player's HP is " << HP << "\r";
			EventManager<HpChangedEvent>::triggerEvent({ HP });
		}
		else if (oppositeType == objectTypes::RADIOACTIVE_WALL && lastWallCollistion < 0.1f) {
			HP -= 20;
			lastWallCollistion = 1.0f;
			EventManager<HpChangedEvent>::triggerEvent({ HP });
		}
	}

	void shot()
	{
		MeshRenderer* meshRenderer;
		Mesh* sphereMesh = MeshMaker::makeSphere();
	}

	void stopPlayer() {
		gameStopped = true;
		transform->setVelocityBT(btVector3(0.f, 0.f, 0.f));
	}
};