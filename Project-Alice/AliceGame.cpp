#include "AliceGame.hpp"

AliceGame::AliceGame() : Game() { }

AliceGame::~AliceGame() { }

bool AliceGame::Start() {
	if (!Game::Start()) return false;


	return true;
}

void AliceGame::Update() { 

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