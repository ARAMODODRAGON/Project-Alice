#include "DebugMenu.hpp"

RTTR_REGISTRATION {
	registration::class_<DebugMenu>("DebugMenu")
	.public_object_constructor;
}

DebugMenu::DebugMenu() : drawUI(false), ui(nullptr) { }

DebugMenu::~DebugMenu() { }

void DebugMenu::Start() { 

	// make sure we add the UIRenderer component
	ui = AddComponent<UIRenderer>();
	if (!ui) {
		DEBUG_ERROR("Failed to create UIRenderer!");
		SetIsActive(false);
		return;
	}

}

void DebugMenu::LateUpdate() { 

	// toggle debug menu
	if (Keyboard::GetKey(KeyCode::KeyD).Pressed()) {
		drawUI = !drawUI;
		DEBUG_LOG("TOGGLED!");
	}

	// now draw if it can
	if (drawUI) {

		DEBUG_LOG("Cant draw anything because im waiting on chris to finish that.");
		// first get the fps and draw it
		//float fps = 1.0f / Game::Get()->GetTimer()->GetDeltaTime();
		//ui->DrawText("FPS: " + VTOS(fps), 10.0f, 10.0f, 1.0f, 1.0f, vec3(1.0f));

	}

}
