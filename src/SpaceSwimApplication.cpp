#include "engine/Core.h"

#include "StartMenuScene/StartMenuScene.h"
#include "Manager/GameManager.h"
#include "Manager/ResourceManager.h"
#include "GameScene/Stage_1.h"

#include "GameScene/Script/ObstacleScript.h"
//******************************************************************
// bullet

class SpaceSwimApplication : public Application {
public:
	SpaceSwimApplication(const char* title) : Application(title) {};
private:
	void init() {
		Application::init();

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ResourceManager::addFont("consola 20", io.Fonts->AddFontFromFileTTF("fonts/consola.ttf", 20.f));
		ResourceManager::addFont("consola 40", io.Fonts->AddFontFromFileTTF("fonts/consola.ttf", 40.f));
		ResourceManager::addFont("consola 60", io.Fonts->AddFontFromFileTTF("fonts/consola.ttf", 60.f));

		glClearColor(39 / 255.0f, 40 / 255.0f, 34 / 255.0f, 1.0f);	// set clear color
		glEnable(GL_BLEND);
		glEnable(GL_CULL_FACE);										// turn on backface culling
		glEnable(GL_DEPTH_TEST);									// turn on depth tests
		glEnable(GL_TEXTURE_2D);									// enable texturing
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glActiveTexture(GL_TEXTURE0);								// notify GL the current texture slot is 0

		ResourceManager::createTexture("Milky_Way", "textures/Milky_Way.jpg");
		ResourceManager::createTexture("venus", "textures/venus.jpg");

		Scene* scene = new StartMenuScene();
		SceneManager::loadScene(scene);
	}

	void update() {
		if (GameManager::getChanged()) changeScene();

		if (GameManager::dynamicsWorld)
		{
			GameManager::dynamicsWorld->stepSimulation(Time::delta(), 10);
			int numManifolds = GameManager::dynamicsWorld->getDispatcher()->getNumManifolds();
			for (int i = 0; i < numManifolds; i++)
			{
				btPersistentManifold* contactManifold = GameManager::dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
				btCollisionObject* obA = (btCollisionObject*)(contactManifold->getBody0());
				btCollisionObject* obB = (btCollisionObject*)(contactManifold->getBody1());
				CustomRigidBody* obAA = (CustomRigidBody*)btRigidBody::upcast(obA);
				CustomRigidBody* obBB = (CustomRigidBody*)btRigidBody::upcast(obB);
				// std::cout << obAA->objectType << " " << obBB->objectType << std::endl;
				if (obBB->objectType == objectTypes::PLAYER && obAA->objectType == objectTypes::METEOR)
				{
					ObstacleScript* tmp = (ObstacleScript*)(obAA->gameObject->getComponent<ScriptLoader>()->getScripts().at(0));
					tmp->collide();
				}
				else if (obAA->objectType == objectTypes::PLAYER && obBB->objectType == objectTypes::METEOR)
				{
					ObstacleScript* tmp = (ObstacleScript*)(obBB->gameObject->getComponent<ScriptLoader>()->getScripts().at(0));
					tmp->collide();
				}
			}
		}

		Application::update();
		if (Input::getKeyDown(GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(_window, GL_TRUE);
		}
	}

	void fixedUpdate() {
		Application::fixedUpdate();
	}

	void terminate() {
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