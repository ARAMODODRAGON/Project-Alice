#ifndef _CORE_WINDOW_HPP
#define _CORE_WINDOW_HPP
#include "../General.hpp"

struct SDL_Window;

namespace ALC {

	class Window {

		// window & opengl
		SDL_Window* window;
		void* glContext;
		uvec2 screenSize;
		string windowTitle;

	public:

		Window(const string& windowTitle_, const uvec2& screenSize_);
		~Window();

		// events
		void ClearScreen(const vec4& color);
		void SwapBuffers();

		// getters & setters
		vec2 GetScreenSize() const { return screenSize; }

	};

}

#endif // !_CORE_WINDOW_HPP