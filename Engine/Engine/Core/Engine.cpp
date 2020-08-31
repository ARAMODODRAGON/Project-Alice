#include "Engine.hpp"
#include <SDL.h>
#include <glew.h>
#include <iostream>
#undef ERROR
#include "Debugger.hpp"

#define FIXED_FPS 60

Engine::Engine()
	: isRunning(false)
	, shouldQuit(false)
	, counts_per_frame { 0 }
	, counts_per_second { 0 }
	, lastCounts { 0 }
	, currentCounts { 0 }
	, window(nullptr)
	, glContext(nullptr)
	, screenSizeX(1280)
	, screenSizeY(720) { }

Engine::~Engine() { }

void Engine::Run() {
	if (isRunning) {
		WARNING("The game is already running!");
		return;
	}

	isRunning = true;
	Start();

	while (isRunning && !shouldQuit) {

		PollEvents();

		Update();

		Draw();

		WaitForEndOfFrame();

	}

	Exit();
}

void Engine::Start() {

	#pragma region SDL & Window initialization

	// initialize sdl
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		FATAL_ERROR("Failed to init SDL");
		Quit();
		return;
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
		"test",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		screenSizeX,
		screenSizeY,
		SDL_WINDOW_OPENGL
	);

	// check if the window was created
	if (!window) {
		FATAL_ERROR("Failed to create window");
		Quit();
		return;
	}

	// create context
	glContext = SDL_GL_CreateContext(window);


	// set an attribute
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);


	// check if it was created
	GLenum error = glewInit();
	if (error != GLEW_OK) {
		FATAL_ERROR("Failed to initialize GLEW");
		Quit();
		return;
	}

	// enable this shit
	glEnable(GL_DEPTH_TEST);

	// print graphics card and opengl version
	LOG("Graphics card: " + string(reinterpret_cast<const char*>(glGetString(GL_RENDERER))));
	LOG("OpenGL version: " + string(reinterpret_cast<const char*>(glGetString(GL_VERSION))));

	#pragma endregion

	// get the count frequency
	if (!QueryPerformanceFrequency(&counts_per_second)) {
		ERROR("System does not support a high-resolution performance counter. Quitting...");
		Quit();
	} else {
		counts_per_frame.QuadPart = counts_per_second.QuadPart / FIXED_FPS;
	}

}

void Engine::PollEvents() {
	static SDL_Event ev;
	while (SDL_PollEvent(&ev)) {

		switch (ev.type) {
			case SDL_QUIT: Quit(); break;
			default: break;
		}
	}
}

void Engine::Update() {


}

void Engine::Draw() {
	// clear screen
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void Engine::WaitForEndOfFrame() {

	// wait for end of frame
	lastCounts = currentCounts;
	QueryPerformanceCounter(&currentCounts);
	while ((currentCounts.QuadPart - lastCounts.QuadPart) < counts_per_frame.QuadPart)
		QueryPerformanceCounter(&currentCounts);


	std::cout << "FPS: " << std::to_string(double(counts_per_second.QuadPart) / double(currentCounts.QuadPart - lastCounts.QuadPart)) << std::endl;

	// swap buffers
	SDL_GL_SwapWindow(window);

}

void Engine::Exit() {

	// destroy window and context
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window); window = nullptr;

}
