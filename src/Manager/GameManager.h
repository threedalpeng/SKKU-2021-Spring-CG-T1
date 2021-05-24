#pragma once

#include <random>

#include "engine/Core.h"
#include "bullet/src/btBulletCollisionCommon.h"
#include "bullet/src/btBulletDynamicsCommon.h"

//*******************************************************************
// bullet3
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

/*
GameManager is global class

stage
	0: game start menu
	1~9: n-state
	10 : endding scene
*/

class GameManager
{
public:
	static bool setStage(int state);
	static int getStage();

	static bool setScore(int score);
	static int getScore();

	static bool setChanged(bool isChanged);
	static bool getChanged();

	static bool update();

	static float genRandFloat();

	static bool particleFull();
	static void increaseParticle();
	static void decreaseParticle();

	static btDiscreteDynamicsWorld* dynamicsWorld;

	static std::unordered_map<btRigidBody*, GameObject*> rigidBody_gameObejct_list;

	static std::random_device rd;
	static std::mt19937 gen;
	static std::normal_distribution<float> dist;

	static std::vector<GameObject*> removeList;
	static std::vector<btRigidBody*> removeBodyList;
	static void cleanRemoveList();
	static void cleanRemoveBodyList();

	static Shader* basicShader;
	static Shader* depthShader;
	static Shader* debugShader;
	static Shader* textShader;

private:
	static int _stage;
	static int _score;
	static bool _isChanged;
	static int _maxParticle;
	static int _nowParticle;
};

int GameManager::_stage = 0;
int GameManager::_score = 0;
bool GameManager::_isChanged = false;
btDiscreteDynamicsWorld* GameManager::dynamicsWorld = nullptr;
int GameManager::_maxParticle = 400;
int GameManager::_nowParticle = 0;
std::vector<GameObject*> GameManager::removeList = std::vector<GameObject*>();
std::vector<btRigidBody*> GameManager::removeBodyList = std::vector<btRigidBody*>();

std::random_device GameManager::rd = std::random_device();
std::mt19937 GameManager::gen = std::mt19937(GameManager::rd());
std::normal_distribution<float> GameManager::dist = std::normal_distribution<float>(0.0f, 1.0f);

Shader* GameManager::basicShader = nullptr;
Shader* GameManager::depthShader = nullptr;
Shader* GameManager::debugShader = nullptr;
Shader* GameManager::textShader = nullptr;

bool GameManager::setStage(int state)
{
	_stage = state;
	return true;
}

int GameManager::getStage()
{
	return _stage;
}

bool GameManager::setScore(int score)
{
	_score = score;
	return true;
}

int GameManager::getScore()
{
	return _score;
}

bool GameManager::setChanged(bool isChanged)
{
	_isChanged = isChanged;
	return true;
}

bool GameManager::getChanged()
{
	return _isChanged;
}
bool GameManager::update()
{
	return true;
}

float GameManager::genRandFloat()
{
	return dist(gen);
}

bool GameManager::particleFull()
{
	return _nowParticle < _maxParticle;
}

void  GameManager::increaseParticle()
{
	_nowParticle++;
}
void GameManager::decreaseParticle()
{
	_nowParticle--;
}

void GameManager::cleanRemoveList()
{
	for (GameObject* obj : removeList)
	{
		obj->remove();
	}
	removeList.clear();
}

void GameManager::cleanRemoveBodyList()
{
	for (btRigidBody* body : removeBodyList)
	{
		GameManager::dynamicsWorld->removeCollisionObject(body);
	}
	removeBodyList.clear();
}