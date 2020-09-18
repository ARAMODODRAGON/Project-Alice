#include "AliceGame.hpp"
#include <Engine/Input/Keyboard.hpp>

AliceGame::AliceGame()
	: Game()
	, quitTimer(0)
	, max_quit_time(15)
	, objIndex(nullptr) { }

AliceGame::~AliceGame() { }

bool AliceGame::Init() {
	if (!Game::Init()) return false;

	// initialize the indexes
	objIndex = new FileIndex("Resources/Objects");
	
	// initialize the content handler
	ContentHandler::Init("Resources/Textures", "Resources/Shaders");
	// create a level manager
	LevelManager::Init("Resources/Levels", "test_0", objIndex);

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

}

void AliceGame::Draw() {

	GetWindow()->ClearScreen(vec4(0.0f, 0.0f, 0.0f, 1.0f));

	// draw 
	LevelManager::Draw();

	GetTimer()->WaitForEndOfFrame();
	GetWindow()->SwapBuffers();
}

bool AliceGame::Exit() {
	if (!Game::Exit()) return false;

	// delete the level manager
	LevelManager::Exit();
	// delete content handler
	ContentHandler::Exit();

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