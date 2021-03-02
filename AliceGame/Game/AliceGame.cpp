#include "AliceGame.hpp"

AliceGame::AliceGame() { }

AliceGame::~AliceGame() { }

void AliceGame::Init() { }

void AliceGame::Exit() { }

void AliceGame::Step(ALC::Timestep t) { 

	auto lc = ALC::Keyboard::GetKey(ALC::KeyCode::LeftCtrl);
	auto R = ALC::Keyboard::GetKey(ALC::KeyCode::KeyR);

	if (lc.IsHeld() && R.Pressed() || lc.Pressed() && R.IsHeld()) {
		ALC::SceneManager::LoadScene(ALC::SceneManager::GetActiveSceneBindingIndex());
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