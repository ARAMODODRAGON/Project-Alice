#include "BattleCamera.hpp"
#define SIZE_FACTOR (1.0f / 6.5f);

RTTR_REGISTRATION {
	registration::class_<BattleCamera>("BattleCamera")
		.public_object_constructor;
}

BattleCamera::BattleCamera() : cam(nullptr) { }

BattleCamera::~BattleCamera() { }

void BattleCamera::Start() {

	cam = AddComponent<Camera>();
	// just a check so the game doesnt crash
	if (cam == nullptr) {
		DEBUG_ERROR("Couldnt create camera, AddComponent returned nullptr!");
		SetIsActive(false); // disable so the camera is 
		return; // quit early
	}
	vec2 size = Game::Get()->GetWindow()->GetScreenSize() * SIZE_FACTOR;
	cam->SetCameraSize(size);
}

void BattleCamera::LateUpdate() {
	// update the camera size according to the window size
	vec2 screenSize = Game::Get()->GetWindow()->GetScreenSize() * SIZE_FACTOR;
	if (cam->GetCameraSize() != screenSize) {
		cam->SetCameraSize(screenSize);
	}
}
