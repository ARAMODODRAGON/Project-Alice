#include "Game.hpp"

Game::Game() : window(nullptr), timer(nullptr), sysEvents(nullptr), isRunning(false), shouldQuit(false) { }

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

	sysEvents = new SystemEvents(this);

	return true;
}

void Game::Update() { }

void Game::Draw() { }

bool Game::Exit() {

	if (sysEvents) delete sysEvents;
	sysEvents = nullptr;

	if (timer) delete timer;
	timer = nullptr;
	
	if (window) delete window;
	window = nullptr;

	return true;
}
