#include "Game.hpp"

namespace ALC {

	Game::Game() : window(nullptr), timer(nullptr) { }
	Game::~Game() {
		if (window) delete window, window = nullptr;
		if (timer) delete timer, timer = nullptr;
	}

	void Game::__Initialize(const string& title_, const uvec2& windowsize) {
		if (window || timer) return;
		window = new Window(title_, windowsize);
		timer = new Timer();
	}

}