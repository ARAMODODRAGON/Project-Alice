#include "AliceGame.hpp"

void AliceGame::Init() { }

void AliceGame::Exit() { }

void AliceGame::Step(ALC::Timestep t) { 
	static float timer = 0.0f;
	timer += t;
	if (timer > 1.0f) {
		timer -= 1.0f;
		ALC_DEBUG_LOG("FPS =" + VTOS(t.GetFPS()));
	}
}

void AliceGame::PreDraw() { 
	GetWindow()->ClearScreen(ALC_COLOR_BLACK);
}

void AliceGame::Draw() { }

void AliceGame::PostDraw() {
	GetWindow()->SwapBuffers();
	GetTimer()->CheckIfFrameComplete();
}