#include "BDPlayer.hpp"
#include "Engine\Battle\Spells\Attack\BTAttackSpell.hpp"
#include <glm\gtx\vector_angle.hpp>

BDPlayer::BDPlayer() : shcp(nullptr) {
	SetMoveSpeed(70.0f);
	SetSlowSpeed(25.0f);
	GetSprite()->SetScale(vec2(32.0f));
	GetSprite()->SetPivot(vec2(0.5f));
	//SetBattleSpells({ }, nullptr);
}

BDPlayer::~BDPlayer() { }

void BDPlayer::Start() {
	BTPlayer::Start();
	shcp = AddComponent<ShootComponent>();
	shcp->Allocate<BTBullet>(500);
}

void BDPlayer::Update() {
	auto shoot = Keyboard::GetKey(KeyCode::KeyF);

	static float timer = 0.0f;
	float delta = Game::Get()->GetTimer()->GetDelta();

	if (shoot) {
		timer += delta;
	}

	if (shoot.Pressed() || timer > 0.2f) {
		if (!shoot.Pressed()) timer -= 0.2f;

		uint32 count = 10;
		uint32 i = 0;
		shcp->Shoot<BTBullet>(count, [&](BTBullet* b) {
			b->SetSprite("Flashing Grey Orb", glm::vec2(1));
			b->SetPosition(GetPosition());
			auto* coll = b->AddComponent<CircleCollider>();

			vec3 vel = glm::rotateZ(vec3(0, 1, 0), glm::radians(float(360 / count) * float(i)));
			b->SetVelocity(vec2(vel.x, vel.y) * 60.0f);

			i++;
		});
	}

	// moves player
	BTPlayer::Update();
}

void BDPlayer::LateUpdate() {
	// clamps player within the battle area
	BTPlayer::LateUpdate();
}
