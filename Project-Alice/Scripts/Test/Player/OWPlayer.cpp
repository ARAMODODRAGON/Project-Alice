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

	SetPosition(vec2(1.0f, 0.0f));

	// add components
	sprite = AddComponent<Sprite>();
	sprite->SetPivot(vec2(8.0f, 8.0f)); // center pivot
	sprite->SetScale(vec2(0.4f));
	coll = AddComponent<CircleCollider>();
	coll->SetRadius(4.0f);

	// create another object
	Object* o = Make();
	Sprite* s = o->AddComponent<Sprite>();
	s->SetPivot(vec2(8.0f, 8.0f)); // center pivot
	CircleCollider* c = o->AddComponent<CircleCollider>();
	c->SetRadius(8.0f);

	// create a camera
	cam = Make<Object>()->AddComponent<Camera>();
	vec2 size = Game::Get()->GetWindow()->GetScreenSize() / 6.5f;
	cam->SetCameraSize(size);
}

void OWPlayer::Update() {
	vec2 direction = vec2(0.0f);
	const static float delta = (1.0f / 60.0f);
	const static float move_delta = delta * 7.0f;
	const static float speed = 10.0f;
	//const float slow_value = (Keyboard::GetKey(KeyCode::LeftShift) ? 0.4f : 1.0f);

	const Button UP = Keyboard::GetKey(KeyCode::ArrowUp);
	const Button DOWN = Keyboard::GetKey(KeyCode::ArrowDown);
	const Button RIGHT = Keyboard::GetKey(KeyCode::ArrowRight);
	const Button LEFT = Keyboard::GetKey(KeyCode::ArrowLeft);

	if (UP != DOWN) {
		if (UP) {
			facing = Facing::Up;
			direction.y += 1.0f;
		}
		if (DOWN) {
			facing = Facing::Down;
			direction.y -= 1.0f;
		}
	}
	if (LEFT != RIGHT) {
		if (RIGHT) {
			facing = Facing::Right;
			direction.x += 1.0f;
		}
		if (LEFT) {
			facing = Facing::Left;
			direction.x -= 1.0f;
		}
	}

	if (direction.x == 0.0f && direction.y == 0.0f)
		SetVelocity(vec2(0.0f));
	else
		SetVelocity(glm::normalize(direction) * speed * move_delta);

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
	// rotate sprite
	sprite->SetRotation(sprite->GetRotation() + 0.2f);

	//vec2 velocity = GetVelocity();
	//
	//if (velocity.x == 0.0f && velocity.y == 0.0f) {
	//	switch (facing) {
	//		case OWPlayer::Facing::Right:
	//			sprite->SetTilingIndex(7);
	//			break;
	//		case OWPlayer::Facing::Up:
	//			sprite->SetTilingIndex(10);
	//			break;
	//		case OWPlayer::Facing::Down:
	//			sprite->SetTilingIndex(1);
	//			break;
	//		case OWPlayer::Facing::Left:
	//			sprite->SetTilingIndex(4);
	//			break;
	//		default: break;
	//	}
	//	animTimer = 0;
	//} else {
	//	animTimer++;
	//	#define FRAME_TIMER (animTimer / 8)
	//	if (velocity.x < 0.0f) {
	//		sprite->SetTilingIndex(FRAME_TIMER % 3 + 3);
	//	} else if (velocity.x > 0.0f) {
	//		sprite->SetTilingIndex(FRAME_TIMER % 3 + 6);
	//	} else if (velocity.y > 0.0f) {
	//		sprite->SetTilingIndex(FRAME_TIMER % 3 + 9);
	//	} else if (velocity.y < 0.0f) {
	//		sprite->SetTilingIndex(FRAME_TIMER % 3);
	//	}
	//}
}

void OWPlayer::OnCollisionEnter(const CollisionData& data) {
	DEBUG_LOG("Collision Enter Called!");
}

void OWPlayer::OnCollisionExit(const CollisionData& data) {
	DEBUG_LOG("Collision Exit Called!");
}

