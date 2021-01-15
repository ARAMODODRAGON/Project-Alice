#include "Game.hpp"

namespace ALC {

	Game::Game() : m_window(nullptr), m_timer(nullptr) { }
	Game::~Game() {
		if (m_window) m_window = nullptr;
		if (m_timer) m_timer = nullptr;
	}

	void Game::__Initialize(Window* window_, Timer* timer_) {
		if (m_window || m_timer) return;
		m_window = window_;
		m_timer = timer_;
	}

}