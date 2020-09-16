#include "OWPlayer.hpp"
#include <Engine/Common.hpp>
#include "../Projectile.hpp"

RTTR_REGISTRATION {
	registration::class_<OWPlayer>("OWPlayer")
		.public_object_constructor
		.property("shootColor", &OWPlayer::shootColor)
		.property("shootSpeed", &OWPlayer::shootSpeed)
		.property("shootDestroyTimer", &OWPlayer::shootDestroyTimer);
}

OWPlayer::OWPlayer()
	: sprite(nullptr)
	, cam(nullptr)
	, colorMix(0.0f)
	, increaseMix(true)
	, animTimer(0)
	, facing(Facing::Down)
	, shootColor(1.0f)
	, shootSpeed(1.0f) { }

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
	sprite->SetLayer(-2);

	// create a camera
	cam = Make<Object>()->AddComponent<Camera>();
	vec2 size = GameContext::game->GetWindow()->GetScreenSize() / 65u;
	cam->SetCameraSize(size);
}

void OWPlayer::Update() {
	vec2 velocity = vec2(0.0f);
	const static float delta = (1.0f / 60.0f);
	const static float move_delta = delta * 7.0f;
	//const float slow_value = (Keyboard::GetKey(KeyCode::LeftShift) ? 0.4f : 1.0f);

	const Button UP = Keyboard::GetKey(KeyCode::ArrowUp);
	const Button DOWN = Keyboard::GetKey(KeyCode::ArrowDown);
	const Button RIGHT = Keyboard::GetKey(KeyCode::ArrowRight);
	const Button LEFT = Keyboard::GetKey(KeyCode::ArrowLeft);

	if (UP != DOWN) {
		if (UP) {
			facing = Facing::Up;
			velocity.y += 1.0f;
		}
		if (DOWN) {
			facing = Facing::Down;
			velocity.y -= 1.0f;
		}
	}
	if (LEFT != RIGHT) {
		if (RIGHT) {
			facing = Facing::Right;
			velocity.x += 1.0f;
		}
		if (LEFT) {
			facing = Facing::Left;
			velocity.x -= 1.0f;
		}
	}

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

	if (Keyboard::GetKey(KeyCode::KeyC).Pressed()) {
		// create a projectile
		if (Projectile* proj = Make<Projectile>()) {
			// set sprite properties
			proj->GetSprite()->SetColor(shootColor);
			proj->GetSprite()->SetLayer(1);

			// set physics
			proj->SetPosition(GetPosition());
			proj->SetDestroyTime(shootDestroyTimer);
			switch (facing) {
				case OWPlayer::Facing::Right:
					proj->SetVelocity(vec2(1.0f, 0.0f) * shootSpeed);
					break;
				case OWPlayer::Facing::Up:
					proj->SetVelocity(vec2(0.0f, 1.0f) * shootSpeed);
					break;
				case OWPlayer::Facing::Down:
					proj->SetVelocity(vec2(0.0f, -1.0f) * shootSpeed);
					break;
				case OWPlayer::Facing::Left:
					proj->SetVelocity(vec2(-1.0f, 0.0f) * shootSpeed);
					break;
				default: break;
			}
		}
	}
}

void OWPlayer::LateUpdate() {
	vec2 velocity = GetVelocity();

	if (velocity.x == 0.0f && velocity.y == 0.0f) {
		switch (facing) {
			case OWPlayer::Facing::Right:
				sprite->SetTilingIndex(7);
				break;
			case OWPlayer::Facing::Up:
				sprite->SetTilingIndex(10);
				break;
			case OWPlayer::Facing::Down:
				sprite->SetTilingIndex(1);
				break;
			case OWPlayer::Facing::Left:
				sprite->SetTilingIndex(4);
				break;
			default: break;
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
			sprite->SetTilingIndex(FRAME_TIMER % 3 + 9);
		} else if (velocity.y < 0.0f) {
			sprite->SetTilingIndex(FRAME_TIMER % 3);
		}
	}
}

