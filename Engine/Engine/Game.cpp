#include "Game.hpp"
#include <SDL_events.h>

Game::Game() : window(nullptr), timer(nullptr), isRunning(false), shouldQuit(false) { }

Game::~Game() { }

void Game::Run() {
	if (isRunning) {
		DEBUG_WARNING("Game is already running!");
		return;
	}

	isRunning = true;
	if (!Start()) {
		DEBUG_ERROR("Failed to start correctly. Quitting...");
		isRunning = false;
	}

	while (isRunning && !shouldQuit) {

		PollEvents();

		Update();

		Draw();

	}

	if (!Exit()) {
		DEBUG_ERROR("Failed to exit correctly. Quitting...");
	}
}

bool Game::Start() {

	window = new Window();
	if (!window->Initialize("Game", uvec2(1280, 720)))
		return false;

	timer = new Timer();

	return true;
}

void Game::Update() { }

void Game::Draw() { }

bool Game::Exit() {
	return true;
}

void Game::PollEvents() {
	static SDL_Event e;
	while (SDL_PollEvent(&e)) {
		switch (e.type) {
			case SDL_QUIT: Quit(); break;
			default: break;
		}
	}
}