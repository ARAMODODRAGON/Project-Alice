#include "AliceGame.hpp"
#include <Engine/Keyboard.hpp>

AliceGame::AliceGame() : Game(), quitTimer(0), max_quit_time(60) { }

AliceGame::~AliceGame() { }

bool AliceGame::Start() {
	if (!Game::Start()) return false;


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


	return true;
}




int main(int argc, char* argv[]) {

	Game* game = new AliceGame();
	game->Run();
	delete game;

	return 0;
}