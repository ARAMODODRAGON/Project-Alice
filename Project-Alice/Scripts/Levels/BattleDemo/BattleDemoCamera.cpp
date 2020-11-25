#include "BattleDemoCamera.hpp"
#include <Engine\Battle\BattleManager.hpp>
#include <Engine\Input\Keyboard.hpp>
#include <Engine\Game.hpp>

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
		ui->AddSprite("default");
	}
}

void BattleDemoCamera::Update() { }

void BattleDemoCamera::LateUpdate() {
	const Rect levelBounds = BattleManager::GetBattleArea();
	constexpr float width = 3.0f;

	vec2 ratio = Game::Get()->GetWindow()->GetScreenSize() / camera->GetCameraSize() * 2.0f;

	// draw the level boundries
	Rect left;
	left.bottom = levelBounds.bottom;
	left.top = levelBounds.top;
	left.right = levelBounds.left;
	left.left = left.right - width;
	left.min *= ratio;
	left.max *= ratio;
	ui->DrawSprite("default", left.left, left.bottom, left.right - left.left, left.top - left.bottom);

	Rect right;
	right.bottom = levelBounds.bottom;
	right.top = levelBounds.top;
	right.left = levelBounds.right;
	right.right = right.left + width;
	right.min *= ratio;
	right.max *= ratio;
	ui->DrawSprite("default", right.left, right.bottom, right.right - right.left, right.top - right.bottom);

	Rect top;
	top.left = levelBounds.left;
	top.right = levelBounds.right;
	top.bottom = levelBounds.top;
	top.top = top.bottom + width;
	top.min *= ratio;
	top.max *= ratio;
	ui->DrawSprite("default", top.left, top.bottom, top.right - top.left, top.top - top.bottom);

	Rect bottom;
	bottom.left = levelBounds.left;
	bottom.right = levelBounds.right;
	bottom.top = levelBounds.bottom;
	bottom.bottom = bottom.top - width;
	bottom.min *= ratio;
	bottom.max *= ratio;
	ui->DrawSprite("default", bottom.left, bottom.bottom, bottom.right - bottom.left, bottom.top - bottom.bottom);

}

void BattleDemoCamera::OnDestroy() { }
