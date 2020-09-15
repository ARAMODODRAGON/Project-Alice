#include "OWPlayer.hpp"
#include <Engine/Common.hpp>

RTTR_REGISTRATION {
	registration::class_<OWPlayer>("OWPlayer")
		.public_object_constructor;
}

OWPlayer::OWPlayer()
	: sprite(nullptr)
	, cam(nullptr)
	, colorMix(0.0f)
	, increaseMix(true)
	, animTimer(0)
	, facingUp(false) { }

OWPlayer::~OWPlayer() { }

void OWPlayer::Start() {

	// add components
	sprite = AddComponent<Sprite>();
	sprite->SetPivot(vec2(4.5f, 14.5f)); // center pivot
	//sprite->SetColor(vec4(1.0f, 0.0f, 0.0f, 1.0f));
	//sprite->LoadTexture("Test 0");
	sprite->LoadTexture("Test Character");
	sprite->SetTilingSize(vec2(17.0f, 29.0f));
	sprite->SetTilingOffset(vec2(4.0f, 7.0f));
	sprite->SetTilingMargin(vec2(7.0f, 7.0f));

	// create a camera
	cam = AddComponent<Camera>();
	vec2 size = GameContext::game->GetWindow()->GetScreenSize() / 65u;
	cam->SetCameraSize(size);
}

void OWPlayer::Update() {
	vec2 velocity = vec2(0.0f);
	const static float delta = (1.0f / 60.0f);
	const static float move_delta = delta * 7.0f;
	//const float slow_value = (Keyboard::GetKey(KeyCode::LeftShift) ? 0.4f : 1.0f);

	if (Keyboard::GetKey(KeyCode::ArrowUp)) velocity.y += 1.0f;
	if (Keyboard::GetKey(KeyCode::ArrowDown)) velocity.y -= 1.0f;
	if (Keyboard::GetKey(KeyCode::ArrowRight)) velocity.x += 1.0f;
	if (Keyboard::GetKey(KeyCode::ArrowLeft)) velocity.x -= 1.0f;

	if (glm::length(velocity) > 0.0f)
		SetPosition(GetPosition() + glm::normalize(velocity) * move_delta);
	SetVelocity(velocity);

	if (Keyboard::GetKey(KeyCode::KeyW)) {
		vec2 size = cam->GetCameraSize();
		size *= (1.0f - delta);
		cam->SetCameraSize(size);
	} else if (Keyboard::GetKey(KeyCode::KeyS)) {
		vec2 size = cam->GetCameraSize();
		size *= (1.0f + delta);
		cam->SetCameraSize(size);
	}

	//const static float color_delta = delta * 0.33f;
	//if (increaseMix) {
	//	colorMix += color_delta;
	//	if (colorMix > 1.0f) {
	//		colorMix = 1.0f;
	//		increaseMix = false;
	//	}
	//} else {
	//	colorMix -= color_delta;
	//	if (colorMix < 0.0f) {
	//		colorMix = 0.0f;
	//		increaseMix = true;
	//	}
	//}
	//
	//#define BRIGHT 0.1f
	//const static vec4 primary_color = vec4(1.0f, BRIGHT, BRIGHT, 1.0f);
	//const static vec4 secondary_color = vec4(BRIGHT, BRIGHT, 1.0f, 1.0f);
	//sprite->SetColor(glm::mix(primary_color, secondary_color, colorMix));
}

void OWPlayer::LateUpdate() {
	vec2 velocity = GetVelocity();

	if (velocity.x == 0.0f && velocity.y == 0.0f) {
		if (facingUp) {
			sprite->SetTilingIndex(10);
		} else {
			sprite->SetTilingIndex(1);
		}
		animTimer = 0;
	} else {
		animTimer++;
		#define FRAME_TIMER (animTimer / 8)
		if (velocity.x < 0.0f) {
			sprite->SetTilingIndex(FRAME_TIMER % 3 + 3);
		} else if (velocity.x > 0.0f) {
			sprite->SetTilingIndex(FRAME_TIMER % 3 + 6);
		} else if (velocity.y > 0.0f) {
			facingUp = true;
			sprite->SetTilingIndex(FRAME_TIMER % 3 + 9);
		} else if (velocity.y < 0.0f) {
			facingUp = false;
			sprite->SetTilingIndex(FRAME_TIMER % 3);
		}
	}
}

