#include "AliceGame.hpp"
#include <Engine/Input/Keyboard.hpp>

AliceGame::AliceGame()
	: Game()
	, quitTimer(0)
	, max_quit_time(15)
	, objIndex(nullptr)
	, content(nullptr)
	, levelManager(nullptr) { }

AliceGame::~AliceGame() { }

bool AliceGame::Start() {
	if (!Game::Start()) return false;

	// initialize the indexes
	objIndex = new FileIndex("ObjectAssets/Objects");
	
	// initialize the content handler
	content = new ContentHandler("Resources/Textures", "ObjectAssets/Shaders");

	// create a level manager
	levelManager = new LevelManager("ObjectAssets/Levels", "test_0", objIndex, content);

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

	// update 
	levelManager->GetLevel()->Update();

}

void AliceGame::Draw() {

	GetWindow()->ClearScreen(vec4(0.0f, 0.0f, 0.0f, 1.0f));

	// draw 
	levelManager->GetLevel()->Draw();

	GetTimer()->WaitForEndOfFrame();
	GetWindow()->SwapBuffers();
}

bool AliceGame::Exit() {
	if (!Game::Exit()) return false;

	// delete the level manager
	if (levelManager) delete levelManager;
	levelManager = nullptr;

	// delete content handler
	if (content) delete content;
	content = nullptr;

	// delete the indexes
	if (objIndex) delete objIndex;
	objIndex = nullptr;

	return true;
}




int main(int argc, char* argv[]) {

	Game* game = new AliceGame();
	game->Run();
	delete game;

	return 0;
}