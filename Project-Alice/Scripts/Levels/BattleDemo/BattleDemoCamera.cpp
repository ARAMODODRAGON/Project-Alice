#include "BattleDemoCamera.hpp"
#include <Engine\Battle\BattleManager.hpp>
#include <Engine\Input\Keyboard.hpp>

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
	//Rect levelBounds = BattleManager::GetBattleArea();
	//constexpr float width = 10.0f;

	// draw the level boundries
	//Rect left;
	//left.min.y = levelBounds.min.y;
	//left.max.y = levelBounds.max.y;
	//left.max.x = levelBounds.min.x;
	//left.min.x = left.max.x - width;
	//ui->DrawSprite("default", left.min.x, left.min.y, left.max.x, left.max.y);
	//
	//Rect right;
	//right.min.y = levelBounds.min.y;
	//right.max.y = levelBounds.max.y;
	//right.max.x = levelBounds.max.x + width;
	//right.min.x = right.max.x;
	//ui->DrawSprite("default", right.min.x, right.min.y, right.max.x, right.max.y);
	//
	//Rect top;
	//top.min.x = levelBounds.min.x;
	//top.max.x = levelBounds.max.x;
	//top.min.y = levelBounds.max.y;
	//top.max.y = top.min.y + 10;
	//ui->DrawSprite("default", top.min.x, top.min.y, top.max.x, top.max.y);
	//
	//Rect bottom;
	//bottom.min.x = levelBounds.min.x;
	//bottom.max.x = levelBounds.max.x;
	//bottom.max.y = levelBounds.min.y;
	//bottom.min.y = bottom.max.y - 10;
	//ui->DrawSprite("default", bottom.min.x, bottom.min.y, bottom.max.x, bottom.max.y);

}

void BattleDemoCamera::OnDestroy() { }
