#ifndef ALC_CORE_GAME_HPP
#define ALC_CORE_GAME_HPP
#include "Window.hpp"
#include "Timer.hpp"

namespace ALC {

	class Game {
	public:

		Game();
		~Game();

		virtual void Init() = 0;
		virtual void Exit() = 0;
		virtual void Step(Timestep t) = 0;
		virtual void PreDraw() = 0;
		virtual void Draw() = 0;
		virtual void PostDraw() = 0;

		Window* GetWindow() { return m_window; }
		Timer* GetTimer() { return m_timer; }

	public:
		void __Initialize(Window* window_, Timer* timer_);
	private:
		Window* m_window;
		Timer* m_timer;
	};

}

#endif // !ALC_CORE_GAME_HPP