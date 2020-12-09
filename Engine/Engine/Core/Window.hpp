#ifndef _CORE_WINDOW_HPP
#define _CORE_WINDOW_HPP
#include "../General/Math.hpp"
#include "../General/Types.hpp"

class Window {

	// window & opengl
	struct SDL_Window* window;
	void* glContext;
	uvec2 screenSize;
	string windowTitle;

public:

	Window();
	~Window();

	// events
	bool Initialize(const string& windowTitle_, const uvec2& screenSize_);
	void ClearScreen(const vec4& color);
	void SwapBuffers();

	// getters & setters
	vec2 GetScreenSize() const { return screenSize; }
	void SetScreenSize(const uvec2& newsize);

};

#endif // !_CORE_WINDOW_HPP