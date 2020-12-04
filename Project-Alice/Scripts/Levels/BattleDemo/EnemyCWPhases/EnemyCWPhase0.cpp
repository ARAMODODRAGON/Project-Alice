#include "EnemyCWPhase0.hpp"

RTTR_REGISTRATION {
	registration::class_<EnemyCWPhase0>("EnemyCWPhase0")
		.public_object_constructor;
}

EnemyCWPhase0::EnemyCWPhase0() : shc(nullptr), i(-1) { }

EnemyCWPhase0::~EnemyCWPhase0() { }

void EnemyCWPhase0::Init() {
	if (shc = GetEnemy()->AddComponent<ShootComponent>()) {
		shc->Allocate(type::get<void>(), 100);

	}

}

void EnemyCWPhase0::StartPhase() {
	DEBUG_LOG("State 0 started");
	state = State::State_Shoot3;

	GetEnemy()->SetMaxHealth(100.0f);
	GetEnemy()->SetCurrentHealth(100.0f);
	BattleManager::SetTimer(20.0f);
}

void EnemyCWPhase0::UpdatePhase() {
	glm::vec2 pos = GetEnemyPosition() - 8.0f;
	float angleDelta = 10;
	float tmpX = -10.0f;
	float tmpY = -10.0f;
	float speed = 10;
	auto& state_ = state;


	if (state == State::State_Shoot3) {
		shc->Shoot(type::get<void>(), 3, [&angleDelta, &pos, &tmpX, &tmpY, &speed, &state_] (BTBullet* b) {
			b->SetPosition(pos);
			b->SetSprite("Flashing Grey Orb", glm::vec2(1));
			b->SetVelocity(vec2(tmpX, tmpY) * speed);
			tmpX += angleDelta;
			state_ = State::State_Move;
		});
	} else if (state == State::State_Move) {


		glm::vec2 vel = GetEnemy()->GetVelocity();
		BTAResult result = bta::MoveTo(&vel, GetEnemy()->GetPosition(), vec2(-100, 100), 200, speed * 20);
		GetEnemy()->SetVelocity(vel);
		if (result == BTAResult::Arrived) {

			shc->Shoot(type::get<void>(), 3, [&angleDelta, &pos, &tmpX, &tmpY, &speed, &state_] (BTBullet* b) {
				b->SetPosition(pos);
				b->SetSprite("Flashing Grey Orb", glm::vec2(1));
				b->SetVelocity(vec2(tmpX, tmpY) * speed);
				tmpX += angleDelta;
				state_ = State::State_Move;
			});

			state = State::State_Center;
		}
	}

	else if (state == State::State_Center) {

		glm::vec2 vel = GetEnemy()->GetVelocity();
		BTAResult result = bta::MoveTo(&vel, GetEnemy()->GetPosition(), vec2(0, 0), 200, speed * 20);
		GetEnemy()->SetVelocity(vel);

		if (result == BTAResult::Arrived) {
			state = State::State_Shoot3;
		}
	}





}

bool EnemyCWPhase0::IsComplete() {
	if (GetEnemy()->GetCurrentHealth() <= 0.0f) {
		DEBUG_LOG("Enemy health was 0");
		return true;
	} else if (BattleManager::GetTimer() <= 0.0f) {
		DEBUG_LOG("Timer was 0");
		return true;
	}
	return false;
}
