#include "BattleCamera.hpp"

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
	cam->SetCameraSize(Game::Get()->GetWindow()->GetScreenSize()) ;
}

void BattleCamera::LateUpdate() {
	// update the camera size according to the window size
	vec2 screenSize = Game::Get()->GetWindow()->GetScreenSize();
	if (cam->GetCameraSize() != screenSize) {
		cam->SetCameraSize(screenSize);
	}
}
