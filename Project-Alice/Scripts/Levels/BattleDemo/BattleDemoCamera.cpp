#include "BattleDemoCamera.hpp"

RTTR_REGISTRATION {
	registration::class_<BattleDemoCamera>("BattleDemoCamera")
		.public_object_constructor;
}

BattleDemoCamera::BattleDemoCamera() : camera(nullptr), ui(nullptr) { }

BattleDemoCamera::~BattleDemoCamera() { }

void BattleDemoCamera::Start() {
	// create camera
	if (camera = AddComponent<Camera>()) {
		// init camera here
		camera->SetCameraSize(vec2(512, 288)); // this is in pixels
	}
	// created ui renderer
	if (ui = AddComponent<UIRenderer>()) {
		// init ui here
	}
}

void BattleDemoCamera::Update() { }

void BattleDemoCamera::LateUpdate() { }

void BattleDemoCamera::OnDestroy() { }
