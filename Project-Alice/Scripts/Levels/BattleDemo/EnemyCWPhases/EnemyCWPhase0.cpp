#include "EnemyCWPhase0.hpp"

EnemyCWPhase0::EnemyCWPhase0() : shc(nullptr), i(-1) { }

EnemyCWPhase0::~EnemyCWPhase0() { }

void EnemyCWPhase0::Init() {
	if (shc = GetEnemy()->AddComponent<ShootComponent>()) {
		shc->Allocate<BTBullet>(100);
	}
}

void EnemyCWPhase0::StartPhase() {
	DEBUG_LOG("State 0 started");
	state = State_Shoot3;

	GetEnemy()->SetMaxHealth(100.0f);
	GetEnemy()->SetCurrentHealth(100.0f);
	SetEnemyPosition(GetEnemy()->GetPosition());
	BattleManager::SetTimer(20.0f);
}

void EnemyCWPhase0::UpdatePhase() {
	glm::vec2 pos = GetEnemyPosition() - 8.0f;
	float angleDelta = 10;
	float tmpX = -10.0f;
	float tmpY = -10.0f;
	float speed = 10;
	auto& state_ = state;
	uint8 dir;



	if (state == State_Shoot3) {
		shc->Shoot<BTBullet>(3, [&angleDelta, &pos, &tmpX, &tmpY, &speed, &state_,&dir] (BTBullet* b) {
			b->SetPosition(pos);
			b->SetSprite("Flashing Grey Orb", glm::vec2(1));

			switch (dir) {
			case 'U':
				break;
			case 'D':
				b->SetVelocity(vec2(tmpX, tmpY) * speed);
				break;
			case 'L':
				break;
			case'R':
				break;
			default:
				b->SetVelocity(vec2(tmpX, tmpY) * speed);
				break;
			}

			//b->SetVelocity(vec2(tmpX, tmpY) * speed);
			tmpX += angleDelta;
			state_ = State_Move;
		});
	} 
	else if (state == State_Move) {


		glm::vec2 vel = GetEnemy()->GetVelocity();
		BTAResult result = bta::MoveTo(&vel, GetEnemy()->GetPosition(), vec2(-100, 100), 500, (speed * 50));
		GetEnemy()->SetVelocity(vel);

		if (result == BTAResult::Arrived) {

			shc->Shoot<BTBullet>(3, [&angleDelta, &pos, &tmpX, &tmpY, &speed, &state_] (BTBullet* b) {
				b->SetPosition(pos);
				b->SetSprite("Flashing Grey Orb", glm::vec2(1));
				b->SetVelocity(vec2(tmpX, tmpY) * speed);
				tmpX += angleDelta;
				state_ = State_Move;
			});

			state = State_Center;
		}
	}

	else if (state == State_Center) {

		glm::vec2 vel = GetEnemy()->GetVelocity();
		BTAResult result = bta::MoveTo(&vel, GetEnemy()->GetPosition(), vec2(0, 0), 500, (speed * 50));
		GetEnemy()->SetVelocity(vel);

		if (result == BTAResult::Arrived) {
			state = State_Shoot3;
		}
	}

}

bool EnemyCWPhase0::IsComplete() {

	/*if (GetEnemy()->GetCurrentHealth() <= 0.0f) {
		DEBUG_LOG("Enemy health was 0");
		return true;
	} else if (BattleManager::GetTimer() <= 0.0f) {
		DEBUG_LOG("Timer was 0");
		return true;
	}	*/
	return false;
}

void EnemyCWPhase0::PlayerDirection(glm::vec2 enemyPos_, uint8 dir_)
{
	
}
