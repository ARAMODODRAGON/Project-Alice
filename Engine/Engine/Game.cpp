#include "Game.hpp"
#include <exception>
#include "Input/SystemEvents.hpp"

Game* Game::_instance = nullptr;

Game::Game() : window(nullptr), timer(nullptr), isRunning(false), shouldQuit(false) {
	if (_instance == nullptr)
		_instance = this;
}
Game::~Game() {
	if (_instance == this)
		_instance = nullptr;
}

void Game::Run() {
	if (_instance != this) {
		DEBUG_ERROR("There are multiple game instances!");
		return;
	}
	if (isRunning) {
		DEBUG_WARNING("Game is already running!");
		return;
	}

	isRunning = true;
	try {
		if (!Init()) {
			DEBUG_ERROR("Failed to start correctly. Quitting...");
			isRunning = false;
		}
	} catch (std::exception& e) {
		DEBUG_ERROR(string("Exception caught while initializing: ") + e.what());
		isRunning = false;
	}

	while (isRunning && !shouldQuit) {

		try {
			Update();
		} catch (std::exception& e) {
			DEBUG_ERROR(string("Exception caught: ") + e.what());
		}

		try {
			Draw();
		} catch (std::exception& e) {
			DEBUG_ERROR(string("Exception caught: ") + e.what());
		}

	}

	try {
		if (!Exit()) {
			DEBUG_ERROR("Failed to exit correctly. Quitting...");
		}
	} catch (std::exception& e) {
		DEBUG_ERROR(string("Exception caught while exiting: ") + e.what());
	}
}

void Game::PollEvents() {
	SystemEvents::PollEvents();
}

bool Game::Init() {

	window = new Window();
	if (!window->Initialize("Game", uvec2(1280, 720)))
		return false;

	timer = new Timer();

	// initialize system events
	SystemEvents::Init(this);

	return true;
}

void Game::Update() { }

void Game::Draw() { }

bool Game::Exit() {

	SystemEvents::Exit();

	if (timer) delete timer;
	timer = nullptr;

	if (window) delete window;
	window = nullptr;

	return true;
}
