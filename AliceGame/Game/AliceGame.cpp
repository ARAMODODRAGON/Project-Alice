#include "AliceGame.hpp"

AliceGame::AliceGame() { }

AliceGame::~AliceGame() { }

void AliceGame::Init() { }

void AliceGame::Exit() { }

void AliceGame::Step(ALC::Timestep t) { 
	using KB = ALC::Keyboard;
	using KC = ALC::KeyCode;

	auto lc = KB::GetKey(KC::LeftCtrl);
	auto R = KB::GetKey(KC::KeyR);

	if (lc && R.Pressed() || lc.Pressed() && R) {
		ALC::SceneManager::LoadLevel(ALC::SceneManager::GetActiveSceneBuildIndex());
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