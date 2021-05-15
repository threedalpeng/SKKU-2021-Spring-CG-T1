#include "engine/Core.h"
#include "irrKlang\irrKlang.h"
#pragma comment(lib, "irrKlang.lib")

#include "StartMenuScene/StartMenuScene.h"
#include "Manager/GameManager.h"
#include "GameScene/Stage_1.h"


//*******************************************************************
// irrKlang objects
irrklang::ISoundEngine* engine;
irrklang::ISoundSource* mp3_src = nullptr;
static const char* mp3_path = "../bin/sounds/space.mp3";
//****************************************************************
// temp val
btRigidBody* body_meteor;
class SpaceSwimApplication : public Application {
public:
	SpaceSwimApplication(const char* title) : Application(title) {};
private:
	void init() {
		Application::init();

		glClearColor(39 / 255.0f, 40 / 255.0f, 34 / 255.0f, 1.0f);	// set clear color
		glEnable(GL_BLEND);
		glEnable(GL_CULL_FACE);										// turn on backface culling
		glEnable(GL_DEPTH_TEST);									// turn on depth tests
		glEnable(GL_TEXTURE_2D);									// enable texturing
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glActiveTexture(GL_TEXTURE0);								// notify GL the current texture slot is 0

		Scene* scene = new StartMenuScene();
		SceneManager::loadScene(scene);

		engine = irrklang::createIrrKlangDevice();
		if (!engine) return;

		//add sound source from the sound file
		mp3_src = engine->addSoundSourceFromFile(mp3_path);

		//set default volume
		mp3_src->setDefaultVolume(0.5f);

		//play the sound file
		engine->play2D(mp3_src, true);
		printf("> playing %s\n", "mp3");

		

	}
	
	void update() {
		if (GameManager::getChanged()) changeScene();

		Application::update();
		if (Input::getKeyDown(GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(_window, GL_TRUE);
		}
	}

	void fixedUpdate() {
		Application::fixedUpdate();
	}

	void terminate() {
		engine->drop();
		Application::terminate();
	}

	void changeScene()
	{
		Scene* scene = nullptr;
		switch (GameManager::getStage())
		{
		case 0:
			scene = new StartMenuScene();
			break;
		case 1:
			scene = new Stage_1();
			break;
		case 2:
			break;
		case 3:
			break;
		default:
			break;
		}
		SceneManager::loadScene(scene);
		GameManager::setChanged(false);
	}
};

int main() {
	SpaceSwimApplication* app = new SpaceSwimApplication("Space Swim");
	app->run();
}