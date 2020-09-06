#include "AliceGame.hpp"
#include <Engine/Keyboard.hpp>

AliceGame::AliceGame()
	: Game()
	, quitTimer(0)
	, max_quit_time(60)
	, entityIndex(nullptr)
	, scriptableIndex(nullptr)
	, levelManager(nullptr)
	, soFactory(nullptr) { }

AliceGame::~AliceGame() { }

bool AliceGame::Start() {
	if (!Game::Start()) return false;

	// initialize the indexes
	entityIndex = new ObjectIndex("Objects/Entities");
	scriptableIndex = new ObjectIndex("Objects/Scriptables");

	// create a level manager
	levelManager = new LevelManager("Objects/Levels", "test_0", entityIndex);

	// create the factory
	soFactory = new ScriptableObjectFactory(scriptableIndex);
	soFactory->Make<ScriptableObject>("Player Stats");

	return true;
}

void AliceGame::Update() {

	// level load actions
	levelManager->DoLevelAction();

	GetSystemEvents()->PollEvents();

	if (Keyboard::GetKey(KeyCode::Escape)) {
		++quitTimer;
		if (quitTimer > max_quit_time) Quit();
	} else {
		quitTimer = 0;
	}

	// cleanup the factory
	soFactory->Cleanup();

	// update 
	soFactory->Update();
	levelManager->GetLevel()->Update();

}

void AliceGame::Draw() {

	GetWindow()->ClearScreen(vec4(0.0f, 0.0f, 0.0f, 1.0f));

	// late update
	soFactory->LateUpdate();

	// draw 
	soFactory->Draw();
	levelManager->GetLevel()->Draw();

	GetTimer()->WaitForEndOfFrame();
	GetWindow()->SwapBuffers();
}

bool AliceGame::Exit() {
	if (!Game::Exit()) return false;

	// delete the factory
	if (soFactory) delete soFactory;
	soFactory = nullptr;

	// delete the level manager
	if (levelManager) delete levelManager;
	levelManager = nullptr;

	// delete the indexes
	if (entityIndex) delete entityIndex;
	entityIndex = nullptr;
	if (scriptableIndex) delete scriptableIndex;
	scriptableIndex = nullptr;

	return true;
}




int main(int argc, char* argv[]) {

	Game* game = new AliceGame();
	game->Run();
	delete game;

	return 0;
}