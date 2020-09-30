#include "AliceGame.hpp"
#include <Engine/Input/Keyboard.hpp>
#include <Engine/Battle/BattleLevel.hpp>
#include <Engine/Battle/BattleManager.hpp>
#include <Engine/Physics/PhysicsScene.hpp>

AliceGame::AliceGame()
	: Game()
	, quitTimer(0)
	, max_quit_time(15) { }

AliceGame::~AliceGame() { }

bool AliceGame::Init() {
	if (!Game::Init()) return false;

	// initialize the singletons
	ContentHandler::Init("Resources/Textures", "Resources/Shaders");
	RenderScene::Init();
	ObjectFactory::Init("Resources/Objects");
	PhysicsScene::Init();
	LevelManager::Init("Resources/Levels", "collision_test");
	BattleManager::Init("Resources/EnemyPhases");

	return true;
}

void AliceGame::Update() {

	PollEvents();

	if (Keyboard::GetKey(KeyCode::Escape)) {
		++quitTimer;
		if (quitTimer > max_quit_time) Quit();
	} else {
		quitTimer = 0;
	}

	// update 
	LevelManager::Update();
	ObjectFactory::Update();

	// do physics
	PhysicsScene::Step();

}

void AliceGame::Draw() {

	// late update
	LevelManager::LateUpdate();
	ObjectFactory::LateUpdate();


	// draw 
	GetWindow()->ClearScreen(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	RenderScene::Draw();

	// cleanup
	ObjectFactory::Cleanup();
	LevelManager::Cleanup();
	ContentHandler::Clean();

	// wait for end of frame
	GetTimer()->WaitForEndOfFrame();
	GetWindow()->SwapBuffers();
}

bool AliceGame::Exit() {
	if (!Game::Exit()) return false;

	// Exit the singletons
	LevelManager::Exit();
	ObjectFactory::Exit();
	PhysicsScene::Exit();
	RenderScene::Exit();
	ContentHandler::Exit();

	return true;
}

void LoadObjects(const json& data) {
	for (auto it = data.begin(); it != data.end(); ++it) {
		if (!it->is_object()) {
			DEBUG_ERROR("Non object in object array! Skipped");
			continue;
		}
		const json& objData = (*it);

		// construct using index
		if (objData.contains("objectName") && objData["objectName"].is_string()) {
			string objectName = objData["objectName"].get<string>();
			Object* o = nullptr;

			// construct with instance data
			if (objData.contains("data") && objData.is_object())
				o = ObjectFactory::Make(objectName, objData["data"]);

			// construct without instance data
			else o = ObjectFactory::Make<Object>(objectName);

			// check
			if (o == nullptr) DEBUG_ERROR("Could not make object of name " + objectName);
			else DEBUG_LOG("Successfully made object of name " + objectName);
		}
		// construct using type
		else if (objData.contains("type") && objData["type"].is_string()) {
			string typName = objData["type"].get<string>();
			type typ = type::get_by_name(typName.c_str());
			Object* o = nullptr;

			// construct with instance data
			if (objData.contains("data") && objData.is_object())
				o = ObjectFactory::Make(typ, objData["data"]);

			// construct without instance data
			else o = ObjectFactory::Make(typ);

			// check
			if (o == nullptr) DEBUG_ERROR("Could not make object of type " + typName);
			else DEBUG_LOG("Successfully made object of type " + typName);
		}
		// if nothing print error
		else {
			DEBUG_ERROR("No Type or ObjectName found");
		}

	}
}

void AliceGame::LevelLoad(Level* level, const json& data) {
	// destroy all objects
	ObjectFactory::Clear();

	// load the level data
	if (data.contains("objects"))
		LoadObjects(data["objects"]);
}

int main(int argc, char* argv[]) {

	Game* game = new AliceGame();
	game->Run();
	delete game;

	return 0;
}