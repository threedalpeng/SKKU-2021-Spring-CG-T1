#include "engine/Core.h"

#include "StartMenuScene/StartMenuScene.h"
#include "Manager/GameManager.h"
#include "Manager/ResourceManager.h"

#include "GameScene/Stage_1.h"
#include "GameScene/Stage_2.h"

#include "GameScene/Script/ObstacleScript.h"
#include "GameScene/Script/PlayerScript.h"
#include "GameScene/Script/BulletScript.h"
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

		glClearColor(0 / 255.0f, 0 / 255.0f, 0 / 255.0f, 0.0f);	// set clear color
		glEnable(GL_BLEND);
		glEnable(GL_CULL_FACE);										// turn on backface culling
		glEnable(GL_DEPTH_TEST);									// turn on depth tests
		glEnable(GL_TEXTURE_2D);									// enable texturing
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glActiveTexture(GL_TEXTURE0);								// notify GL the current texture slot is 0

		ResourceManager::createTexture("Milky_Way", "textures/Milky_Way.jpg");
		ResourceManager::createTexture("venus", "textures/venus.jpg");
		ResourceManager::createTexture("fire", "textures/fire.jpg");
		ResourceManager::createTexture("fireParticle", "textures/fireParticle.png");
		ResourceManager::createTexture("white", "textures/white.jpg");
		ResourceManager::createTexture("Player Head", "textures/spacehead.png");
		ResourceManager::createTexture("mercury", "textures/mercury.jpg");
		ResourceManager::createTexture("moon", "textures/moon.jpg");
		ResourceManager::createTexture("radioactive", "textures/radioactive.png");
		ResourceManager::createTexture("bullet", "textures/bullet.jpg");
		// ResourceManager::createTexture("SpaceShip", "textures/SpaceShip.jpg");
		// ResourceManager::createTexture("monster", "textures/monster.png");
		// ResourceManager::createTexture("asteroid", "textures/asteroid.png");  // too big
		ResourceManager::createTexture("ufo", "textures/ufo.png");

		ResourceManager::createMaterial("Basic");
		// ResourceManager::createShader("basicShader", "shaders/solar-system.vert", "shaders/solar-system.frag");

		ResourceManager::addMesh("Cylinder", MeshMaker::makeCylinderMesh());
		ResourceManager::addMesh("Box", MeshMaker::makeBoxMesh());
		ResourceManager::addMesh("Sky Cylinder", MeshMaker::makeSkyCylinderMesh());
		ResourceManager::addMesh("Sphere", MeshMaker::makeSphere());
		ResourceManager::addMesh("Bullet", new Mesh("mesh/bullet.vertex.bin", "mesh/bullet.index.bin"));
		ResourceManager::addMesh("Flag", new Mesh("mesh/flag.vertex.bin", "mesh/flag.index.bin"));
		// ResourceManager::addMesh("SpaceShip", new Mesh("mesh/SpaceShip.vertex.bin", "mesh/SpaceShip.index.bin"));
		// ResourceManager::addMesh("Monster", new Mesh("mesh/monster.vertex.bin", "mesh/monster.index.bin"));
		// ResourceManager::addMesh("Aonster", new Mesh("mesh/asteroid.vertex.bin", "mesh/asteroid.index.bin"));
		ResourceManager::addMesh("Ufo", new Mesh("mesh/ufo.vertex.bin", "mesh/ufo.index.bin"));

		GameManager::basicShader = new Shader("shaders/solar-system.vert", "shaders/solar-system.frag");
		GameManager::depthShader = new Shader("shaders/depth.vert", "shaders/depth.frag");
		GameManager::debugShader = new Shader("shaders/depthDebug.vert", "shaders/depthDebug.frag");

		Scene* scene = new StartMenuScene();
		SceneManager::loadScene(scene);
	}

	void update() {
		if (GameManager::getChanged()) changeScene();

		// reset section
		glUniform1i(GameManager::basicShader->getUniformLocation("b_shadow"), false);

		GameManager::cleanRemoveList();
		GameManager::cleanRemoveBodyList();

		if (GameManager::dynamicsWorld)
		{
			GameManager::dynamicsWorld->stepSimulation(Time::delta(), 10);
			try {
				collide();
			}
			catch (int expn) {
				std::cout << expn << ": there are error in bullet at collision detection step.\r" << std::endl;
			}
		}

		Application::update();

		if (Input::getKeyDown(GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(_window, GL_TRUE);
		}
	}

	void collide()
	{
		static std::map<int, std::vector<int>> persistentCollisions;
		std::map<int, std::vector<int>> newCollisions;

		int numManifolds = GameManager::dynamicsWorld->getDispatcher()->getNumManifolds();
		for (int i = 0; i < numManifolds; i++)
		{
			btPersistentManifold* contactManifold = GameManager::dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
			btCollisionObject* obA = (btCollisionObject*)(contactManifold->getBody0());
			btCollisionObject* obB = (btCollisionObject*)(contactManifold->getBody1());
			CustomRigidBody* obAA = (CustomRigidBody*)btRigidBody::upcast(obA);
			CustomRigidBody* obBB = (CustomRigidBody*)btRigidBody::upcast(obB);

			int numContacts = contactManifold->getNumContacts();
			for (int j = 0; j < numContacts; j++) {
				btManifoldPoint& pt = contactManifold->getContactPoint(j);
				if (pt.getDistance() < 0.f) {
					// std::cout << obAA->objectType << " " << obBB->objectType << std::endl;
					if (obAA->objectType == objectTypes::METEOR)
					{
						std::vector<Script*> script_v = obAA->gameObject->getComponent<ScriptLoader>()->getScripts();
						for (size_t i = 0; i < script_v.size(); i++)	((ObstacleScript*)script_v.at(i))->collide(obBB->objectType);
					}
					else if (obBB->objectType == objectTypes::METEOR)
					{
						std::vector<Script*> script_v = obBB->gameObject->getComponent<ScriptLoader>()->getScripts();
						for (size_t i = 0; i < script_v.size(); i++)	((ObstacleScript*)script_v.at(i))->collide(obAA->objectType);
					}

					if (obAA->objectType == objectTypes::PLAYER)
					{
						std::vector<Script*> script_v = obAA->gameObject->getComponent<ScriptLoader>()->getScripts();
						for (size_t i = 0; i < script_v.size(); i++)	((PlayerScript*)script_v.at(i))->collide(obBB->objectType);
					}
					else if (obBB->objectType == objectTypes::PLAYER)
					{
						std::vector<Script*> script_v = obBB->gameObject->getComponent<ScriptLoader>()->getScripts();
						for (size_t i = 0; i < script_v.size(); i++)	((PlayerScript*)script_v.at(i))->collide(obAA->objectType);
					}

					if (obAA->objectType == objectTypes::SAVE_POINT)
					{
						std::vector<Script*> script_v = obAA->gameObject->getComponent<ScriptLoader>()->getScripts();
						for (size_t i = 0; i < script_v.size(); i++)	((SavePointScript*)script_v.at(i))->collide(obBB->objectType);
					}
					else if (obBB->objectType == objectTypes::SAVE_POINT)
					{
						std::vector<Script*> script_v = obBB->gameObject->getComponent<ScriptLoader>()->getScripts();
						for (size_t i = 0; i < script_v.size(); i++)	((SavePointScript*)script_v.at(i))->collide(obAA->objectType);
					}

					if (obAA->objectType == objectTypes::BULLET)
					{
						std::vector<Script*> script_v = obAA->gameObject->getComponent<ScriptLoader>()->getScripts();
						for (size_t i = 0; i < script_v.size(); i++)	((BulletScript*)script_v.at(i))->collide(obBB->objectType);
					}
					else if (obBB->objectType == objectTypes::BULLET)
					{
						std::vector<Script*> script_v = obBB->gameObject->getComponent<ScriptLoader>()->getScripts();
						for (size_t i = 0; i < script_v.size(); i++)	((BulletScript*)script_v.at(i))->collide(obAA->objectType);
					}

					if (obAA->objectType == objectTypes::ENEMY_BULLET)
					{
						std::vector<Script*> script_v = obAA->gameObject->getComponent<ScriptLoader>()->getScripts();
						for (size_t i = 0; i < script_v.size(); i++)	((BulletScript*)script_v.at(i))->collide(obBB->objectType);
					}
					else if (obBB->objectType == objectTypes::ENEMY_BULLET)
					{
						std::vector<Script*> script_v = obBB->gameObject->getComponent<ScriptLoader>()->getScripts();
						for (size_t i = 0; i < script_v.size(); i++)	((BulletScript*)script_v.at(i))->collide(obAA->objectType);
					}


					if (obAA->objectType == objectTypes::ENEMY)
					{
						std::vector<Script*> script_v = obAA->gameObject->getComponent<ScriptLoader>()->getScripts();
						for (size_t i = 0; i < script_v.size(); i++)	((EnemyScript*)script_v.at(i))->collide(obBB->objectType);
					}
					else if (obBB->objectType == objectTypes::ENEMY)
					{
						std::vector<Script*> script_v = obBB->gameObject->getComponent<ScriptLoader>()->getScripts();
						for (size_t i = 0; i < script_v.size(); i++)	((EnemyScript*)script_v.at(i))->collide(obAA->objectType);
					}

					break;
				}
			}
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
			scene = new Stage_2();
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