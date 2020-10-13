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

	SetPosition(vec2(16.0f, 0.0f));

	// add components
	sprite = AddComponent<SpriteRenderer>();
	sprite->LoadTexture("circle 128");
	sprite->SetPivot(vec2(128.0f / 2.0f, 128.0f / 2.0f)); // center pivot
	sprite->SetScale(vec2(1.0f / 11.0f));
	sprite->SetLayer(2);
	coll = AddComponent<CircleCollider>();
	coll->SetRadius(4.0f);
	ui = AddComponent<UIRenderer>();

	// create another object
	Object* o = Make();
	SpriteRenderer* s = o->AddComponent<SpriteRenderer>();
	s->LoadTexture("circle 128");
	s->SetPivot(vec2(128.0f / 2.0f, 128.0f / 2.0f)); // center pivot
	s->SetScale(vec2(1.0f / 11.0f));
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
	const static float speed = 300.0f;
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
	//ui->DrawText("THIS IS A TEST", -500.0f, -0.5f, 1.0f, vec3(0.0f, 0.5f, 0.0f));
}

void OWPlayer::OnCollisionEnter(const CollisionData& data) {
	DEBUG_LOG("Collision Enter Called!");
	// set red
	sprite->SetColor(vec4(1.0f, 0.0f, 0.0f, 1.0f));
}

void OWPlayer::OnCollisionExit(const CollisionData& data) {
	DEBUG_LOG("Collision Exit Called!");
	// set white
	sprite->SetColor(vec4(1.0f));
}

