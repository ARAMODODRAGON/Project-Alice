#include "DebugMenu.hpp"

RTTR_REGISTRATION {
	registration::class_<DebugMenu>("DebugMenu")
	.public_object_constructor;
}

DebugMenu::DebugMenu() : drawUI(false), ui(nullptr), multiFrameFPS { 0.0 }, frameIndex(0) { }

DebugMenu::~DebugMenu() { }

void DebugMenu::Start() {

	// make sure we add the UIRenderer component
	ui = AddComponent<UIRenderer>();
	if (!ui) {
		DEBUG_ERROR("Failed to create UIRenderer!");
		SetIsActive(false);
		return;
	}

	// just a little message to let the other know
	DEBUG_LOG("--Use 'D' to toggle debug display--");

}

void DebugMenu::LateUpdate() {

	// toggle debug menu
	if (Keyboard::GetKey(KeyCode::KeyD).Pressed()) {
		drawUI = !drawUI;
		//DEBUG_LOG("TOGGLED!");
	}

	// now draw if it can
	if (drawUI) {

		//DEBUG_LOG("Cant draw anything because im waiting on chris to finish that.");
		// get the fps and draw it
		multiFrameFPS[frameIndex++ % multiFrameFPS.size()] = Game::Get()->GetTimer()->GetFPS();
		vec2 windowsize = Game::Get()->GetWindow()->GetScreenSize();

		// calculate the averagefps
		double average = 0.0f;
		for (double& value : multiFrameFPS) {
			average += value;
		}
		average /= multiFrameFPS.size();

		ui->DrawText("FPS: " + VTOS(Game::Get()->GetTimer()->GetFPS()), -windowsize.x, windowsize.y - 40, 1.0f, 1.0f, vec3(1.0f));

	}

}
