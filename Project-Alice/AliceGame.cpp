#include "AliceGame.hpp"
#include <Engine/Keyboard.hpp>

AliceGame::AliceGame()
	: Game()
	, quitTimer(0)
	, max_quit_time(60)
	, entityIndex(nullptr)
	, levelIndex(nullptr)
	, scriptableIndex(nullptr) { }

AliceGame::~AliceGame() { }

bool AliceGame::Start() {
	if (!Game::Start()) return false;

	// initialize the indexes
	entityIndex = new ObjectIndex("Objects/Entities");
	levelIndex = new ObjectIndex("Objects/Levels");
	scriptableIndex = new ObjectIndex("Objects/Scriptables");

	return true;
}

void AliceGame::Update() {

	GetSystemEvents()->PollEvents();

	if (Keyboard::GetKey(KeyCode::Escape)) {
		++quitTimer;
		if (quitTimer > max_quit_time) Quit();
	} else {
		quitTimer = 0;
	}

}

void AliceGame::Draw() {

	GetWindow()->ClearScreen(vec4(0.0f, 0.0f, 1.0f, 1.0f));


	GetTimer()->WaitForEndOfFrame();
	GetWindow()->SwapBuffers();
}

bool AliceGame::Exit() {
	if (!Game::Exit()) return false;

	// delete the indexes
	if (entityIndex) delete entityIndex;
	entityIndex = nullptr;
	if (levelIndex) delete levelIndex;
	levelIndex = nullptr;
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