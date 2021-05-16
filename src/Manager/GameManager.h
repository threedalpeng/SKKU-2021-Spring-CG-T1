#pragma once
#include "engine/Core.h"
#include "bullet/btBulletCollisionCommon.h"
#include "bullet/btBulletDynamicsCommon.h"

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

	static btDiscreteDynamicsWorld* dynamicsWorld;

	static std::unordered_map<btRigidBody*, GameObject*> rigidBody_gameObejct_list;

private:
	static int _stage;
	static int _score;
	static bool _isChanged;
};

int GameManager::_stage = 0;
int GameManager::_score = 0;
bool GameManager::_isChanged = false;
btDiscreteDynamicsWorld* GameManager::dynamicsWorld = nullptr;

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