#include "BattleDemoCamera.hpp"
#include <Engine\Battle\BattleManager.hpp>
#include <Engine\Input\Keyboard.hpp>
#include <Engine\Game.hpp>

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
		ui->AddSprite("default");
	}
	// create audio component
	if (audio = AddComponent<AudioSourceComponent>()) {
		// init audio here
	}
}

void BattleDemoCamera::Update() { }

void BattleDemoCamera::LateUpdate() {
	//const Rect levelBounds = BattleManager::GetBattleArea();
	//constexpr float width = 3.0f;
	//
	//vec2 ratio = Game::Get()->GetWindow()->GetScreenSize() / camera->GetCameraSize() * 2.0f;
	//
	//// draw the level boundries
	//Rect r;
	//
	//// left
	//r.bottom = levelBounds.bottom - width;
	//r.top = levelBounds.top + width;
	//r.right = levelBounds.left;
	//r.left = r.right - width;
	//r.min *= ratio;
	//r.max *= ratio;
	//ui->DrawSprite("default", r.left, r.bottom, r.right - r.left, r.top - r.bottom);
	//
	//// right
	//r.bottom = levelBounds.bottom - width;
	//r.top = levelBounds.top + width;
	//r.left = levelBounds.right;
	//r.right = r.left + width;
	//r.min *= ratio;
	//r.max *= ratio;
	//ui->DrawSprite("default", r.left, r.bottom, r.right - r.left, r.top - r.bottom);
	//
	//// top
	//r.left = levelBounds.left;
	//r.right = levelBounds.right;
	//r.bottom = levelBounds.top;
	//r.top = r.bottom + width;
	//r.min *= ratio;
	//r.max *= ratio;
	//ui->DrawSprite("default", r.left, r.bottom, r.right - r.left, r.top - r.bottom);
	//
	//// bottom
	//r.left = levelBounds.left;
	//r.right = levelBounds.right;
	//r.top = levelBounds.bottom;
	//r.bottom = r.top - width;
	//r.min *= ratio;
	//r.max *= ratio;
	//ui->DrawSprite("default", r.left, r.bottom, r.right - r.left, r.top - r.bottom);
	//
}

void BattleDemoCamera::OnDestroy() { }
