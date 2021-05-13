#include "engine/Core.h"
#include "StartMenuScene/StartMenuScene.h"
#include "irrKlang\irrKlang.h"
#pragma comment(lib, "irrKlang.lib")

//*******************************************************************
// irrKlang objects
irrklang::ISoundEngine* engine;
irrklang::ISoundSource* mp3_src = nullptr;
static const char*	mp3_path = "../bin/sounds/space.mp3";

class SpaceSwimApplication : public Application {
public:
	SpaceSwimApplication(const char* title, ivec2 window_size) : Application(title, window_size) {};
private:
	void init() {
		Application::init();

		glClearColor(39 / 255.0f, 40 / 255.0f, 34 / 255.0f, 1.0f);	// set clear color
		glEnable( GL_BLEND );
		glEnable(GL_CULL_FACE);										// turn on backface culling
		glEnable(GL_DEPTH_TEST);									// turn on depth tests
		glEnable(GL_TEXTURE_2D);									// enable texturing
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glActiveTexture(GL_TEXTURE0);								// notify GL the current texture slot is 0

		Scene* scene = new StartMenuScene();
		loadScene(scene);

		engine = irrklang::createIrrKlangDevice();
		if (!engine) return;

		//add sound source from the sound file
		mp3_src = engine->addSoundSourceFromFile( mp3_path );
		
		//set default volume
		mp3_src->setDefaultVolume(0.5f);

		//play the sound file
		engine->play2D( mp3_src, true );
		printf( "> playing %s\n", "mp3" );

	}

	void update() {
		Application::update();
		if (Input::getKeyDown(GLFW_KEY_ESCAPE) || Input::getKeyDown(GLFW_KEY_Q)) {
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
};

int main() {
	SpaceSwimApplication* app = new SpaceSwimApplication("Space Swim", cg_default_window_size());
	app->run();
}