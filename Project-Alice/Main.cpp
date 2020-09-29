#include "AliceGame.hpp"
#include <SDL_net.h>

int main(int argc, char* argv[]) {

	//Game* game = new AliceGame();
	//game->Run();
	//delete game;

	if (SDLNet_Init() < 0) {
		DEBUG_ERROR("Failed to initialize SDLNet!");
		return -1;
	}



	SDLNet_Quit();

	return 0;
}