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
		virtual void Step() = 0;
		virtual void PreDraw() = 0;
		virtual void Draw() = 0;
		virtual void PostDraw() = 0;

		Window* GetWindow() { return window; }
		Timer* GetTimer() { return timer; }

	public:
		void __Initialize(const string& title_, const uvec2& windowsize);
	private:
		Window* window;
		Timer* timer;
	};

}

#endif // !ALC_CORE_GAME_HPP