#pragma once

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

    static bool setScore();
    static int getScore();

    static bool setChanged(bool isChanged);
    static bool getChanged();

    static bool update();
    static bool changeScene(void* app);

private:
    static int _stage = 0;
    static int _score = 0;
    static bool _isChanged = false;
};

bool GameManager::setStage(int state)
{
    _stage = stage;
    return true;
} 

int GameManager::getStage()
{
    return _stage;
}

bool GameManager::setSCore(int score)
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

int GameManager::getChanged()
{
    return _isChanged;
}

bool GameManager::update()
{

}

bool GameManager::changeScene(void* app)
{
    Application* _app = (Application*)app;
    Scene* scene;
    switch(_stage)
    {
        case 0:
            scene = new StartMenuScene();
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        default:
            break;
    }
    _app->loadScene(scene);
    setChanged(false);
}