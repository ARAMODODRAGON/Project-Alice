#include "Window.hpp"
#include "Debugger.hpp"
#include <SDL.h>
#include <glew.h>

Window::Window()
	: window(nullptr), glContext(nullptr), screenSize(0) { }

Window::~Window() {
	// destroy window and context
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window); window = nullptr;
}

bool Window::Initialize(const string& windowTitle_, const uvec2& screenSize_) {
	if (window != glContext) {
		// the window has already been initialized
		DEBUG_WARNING("The window has already been initialized");
		return false;
	}

	screenSize = screenSize_;
	windowTitle = windowTitle_;

	#pragma region SDL & Window initialization

	// initialize sdl
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		DEBUG_FATAL_ERROR("Failed to init SDL");
		return false;
	}

	// set some attributes
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// this matches to the refresh rate of the display
	SDL_GL_SetSwapInterval(1);

	// experimental? something something, yea!
	glewExperimental = GL_TRUE;


	// create window
	window = SDL_CreateWindow(
		windowTitle.c_str(),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		screenSize.x,
		screenSize.y,
		SDL_WINDOW_OPENGL
	);

	// check if the window was created
	if (!window) {
		DEBUG_FATAL_ERROR("Failed to create window");
		return false;
	}

	// create context
	glContext = SDL_GL_CreateContext(window);


	// set an attribute
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);


	// check if it was created
	GLenum error = glewInit();
	if (error != GLEW_OK) {
		DEBUG_FATAL_ERROR("Failed to initialize GLEW");
		return false;
	}

	// enable this shit
	glEnable(GL_DEPTH_TEST);

	// print graphics card and opengl version
	DEBUG_LOG("Graphics card: " + string(reinterpret_cast<const char*>(glGetString(GL_RENDERER))));
	DEBUG_LOG("OpenGL version: " + string(reinterpret_cast<const char*>(glGetString(GL_VERSION))));

	#pragma endregion

	// successful
	return true;
}

void Window::ClearScreen(const vec4& color) {
	// clear screen
	glClearColor(color.r, color.g, color.b, color.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::SwapBuffers() {
	// swap buffers
	SDL_GL_SwapWindow(window);
}
