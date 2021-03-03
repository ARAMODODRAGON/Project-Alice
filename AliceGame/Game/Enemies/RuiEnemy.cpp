#include "RuiEnemy.hpp"
#include "../Systems/BulletDeleterSystem.hpp"
#include "../Systems/GravityBulletSystem.hpp"
#include "../Systems/CircleBombSystem.hpp"
#include "../Systems/ZigZagBulletSystem.hpp"
#include"../Systems/Bullet Types/HomingBulletSystem.hpp"
#include "../BTA.hpp"
#include "../Characters/Character.hpp"

RuiEnemy::RuiEnemy()
	: m_phases(this, Phase::PreBattle), m_state(State::None),m_prevState(State::None),m_timer(0.0f),stateTimer(0.0f) {
	// bind phases
	m_phases.Bind(Phase::PreBattle, &RuiEnemy::PreBattleStep, &RuiEnemy::PreBattleBegin);
	m_phases.Bind(Phase::Phase0, &RuiEnemy::Phase0Step, &RuiEnemy::Phase0Begin);
	m_phases.Bind(Phase::Phase1, &RuiEnemy::Phase1Step, &RuiEnemy::Phase1Begin);
	m_phases.Bind(Phase::Phase2, &RuiEnemy::Phase2Step, &RuiEnemy::Phase2Begin);
	m_phases.Bind(Phase::Phase3, &RuiEnemy::Phase3Step, &RuiEnemy::Phase3Begin);
	m_phases.Bind(Phase::Phase4, &RuiEnemy::Phase4Step, &RuiEnemy::Phase4Begin);
	m_phases.Bind(Phase::PostBattle, &RuiEnemy::PostBattleStep, &RuiEnemy::PostBattleBegin);


}

RuiEnemy::~RuiEnemy() { }

void RuiEnemy::Start(ALC::Entity self) {
	Enemy::Start(self); // must be called to initialize
	SetAccel(50 * 20);
	SetSpeed(100 * 20);
	m_bulletTexture = ALC::ContentManager::LoadTexture("Resources/Textures/Spells.png");

	auto& spr = self.GetComponent<ALC::SpriteComponent>();
	spr.texture = ALC::ContentManager::LoadTexture("Resources/Textures/Enemies/Rui-Enemy.png");
	spr.textureBounds = spr.texture.GetBounds();
	spr.bounds = spr.texture.GetCenteredBounds();

	auto* playerb = BattleManager::GetCurrentCharacter();
	auto& playerTransform = playerb->GetEntity().GetComponent<ALC::Transform2D>();
	plyrOldPos = playerTransform.position;
	// match the ratio 
	//float ratio = spr.textureBounds.Height() / spr.textureBounds.Width();
	//float newHeight = spr.bounds.Width() * ratio;
	//spr.bounds.top = newHeight * 0.5f;
	//spr.bounds.bottom = -newHeight * 0.5f;

}

void RuiEnemy::Update(ALC::Entity self, ALC::Timestep ts) {
	// update the phase
	m_phases(self, ts);

	auto* playerb = BattleManager::GetCurrentCharacter();
	auto& playerTransform = playerb->GetEntity().GetComponent<ALC::Transform2D>();
	ALC::vec2 offset = playerTransform.position - plyrOldPos;

	float threshHold = 2.0f;

	// these if checks
	if (offset.x > threshHold || offset.y > threshHold) {
		plyrOldPos = playerTransform.position;
		plyrMoveTimer = 0.0f;
	}

	else if (offset.x < -threshHold || offset.y < -threshHold) {
		plyrOldPos = playerTransform.position;
		plyrMoveTimer = 0.0f;
	}	  

	else {
		plyrMoveTimer += ts.Get();
		ALC::Debugger::Log("move", "", __LINE__);
	}

}

void RuiEnemy::LateUpdate(ALC::Entity self, ALC::Timestep ts) {
	// only do this stuff when the battle actually starts
	if (m_phases.GetState() != Phase::PreBattle ||
		m_phases.GetState() != Phase::PostBattle) {
		// takes damage from all bullets, calls OnDeath when it reaches 0
		UpdateCollisions(self, ts);

		// updates the lifetime, calls OnDeath when it reaches 0
		UpdateLifetime(self, ts);
	}
}

void RuiEnemy::OnDeath(ALC::Entity self) {
	switch (m_phases.GetState()) {
		case Phase::Phase0: m_phases.ChangeState(Phase::Phase1); break;
		case Phase::Phase1: m_phases.ChangeState(Phase::Phase2); break;
		case Phase::Phase2: m_phases.ChangeState(Phase::Phase3); break;
		case Phase::Phase3: m_phases.ChangeState(Phase::Phase4); break;
		case Phase::Phase4: m_phases.ChangeState(Phase::PostBattle); break;
		default: break;
	}
	self.GetComponent<ALC::CharacterBody>().velocity = ALC::vec2();
}

void RuiEnemy::BattleBegin() {
	// we start by changing phases
	m_phases.ChangeState(Phase::Phase0);
}

void RuiEnemy::PreBattleBegin(const Phase lastphase, ALC::Entity self, ALC::Timestep ts) { }
void RuiEnemy::PreBattleStep(ALC::Entity self, ALC::Timestep ts) { }

void RuiEnemy::Phase0Begin(const Phase lastphase, ALC::Entity self, ALC::Timestep ts) {
	ResetHealth(200.0f);
	SetLifetime(5000000.0f);

	if (m_state != State::None) {
		m_prevState = m_state;
	}

	if (!moveStates.empty()) { moveStates.clear(); }

	moveStates.push_back(MoveStates::States::DownLeft);
	moveStates.push_back(MoveStates::States::UpRight);
	moveStates.push_back(MoveStates::States::Left);
	moveStates.push_back(MoveStates::States::Up);
	moveStates.push_back(MoveStates::States::Down);
	moveStates.push_back(MoveStates::States::UpLeft);
	moveStates.push_back(MoveStates::States::Right);

	m_state = State::RainBullets; 

}

void RuiEnemy::Phase0Step(ALC::Entity self, ALC::Timestep ts) {	  

	auto [transform, cbody] = self.GetComponent<ALC::Transform2D, ALC::CharacterBody>();
	auto tex = m_bulletTexture;
	int numOfBullets = 15;
	int bulRange = rand() % 120 + 45;
	float fireRate = 0.3f;
	auto* playerb = BattleManager::GetCurrentCharacter();
	auto& playerTransform = playerb->GetEntity().GetComponent<ALC::Transform2D>();
	//playerb->SetHealth(100000000000.0f);

	if (GetHealth() <= GetMaxHealth() / 2) {
		numOfBullets = 20;
		fireRate = 0.2f;
	}

	if (m_state == State::RainBullets) {
		// set value
		ShooterBehavior::SetDefaultPosition(transform.position);
		
		// target the players position by using this in velocity
		ALC::vec2 targetdir = playerTransform.position - transform.position;
		if (glm::length2(targetdir) > 0.0f) { targetdir = glm::normalize(targetdir); }

		ShooterBehavior::SetDefaultVelocity(targetdir * 300.0f);

		if (transform.position.y <= playerTransform.position.y || plyrMoveTimer >= 1.0f ) {

			if (m_timer <= ts.Get()) {

				ShooterBehavior::ShootRange(self, 7, 25.0f, [tex](ALC::Entity bullet) {
					// update body collision
					auto& body = bullet.GetComponent<ALC::BulletBody>();
					body.radius = 4.0f;
					auto& sprite = bullet.GetComponent<ALC::SpriteComponent>();
					// this should be loaded ahead of time
					sprite.texture = tex;
					sprite.textureBounds = ALC::rect(16.0f, 80.0f, 31.0f, 95.0f);
					sprite.bounds = sprite.textureBounds.Centered();

				}, ALC::BulletTypes<BulletDeleterComponent, NormalBullet>());
			}
		}

		ShooterBehavior::SetDefaultVelocity(dir[0] * 300.0f);

		m_timer += ts.Get();
		stateTimer += ts.Get();

		if (m_timer <= ts.Get()) {

			// uses default velocity to shoot a range
			// if default is [0, 1] then it will shoot in a range from (-45.0f / 2) to (+45.0f / 2)
			ShooterBehavior::ShootRange(self, numOfBullets, bulRange, [tex](ALC::Entity bullet) {
				// update body collision
				auto& body = bullet.GetComponent<ALC::BulletBody>();
				body.radius = 4.0f;
				auto& sprite = bullet.GetComponent<ALC::SpriteComponent>();
				// this should be loaded ahead of time
				sprite.texture = tex;
				sprite.textureBounds = ALC::rect(16.0f, 80.0f, 31.0f, 95.0f);
				sprite.bounds = sprite.textureBounds.Centered();

			}, ALC::BulletTypes<BulletDeleterComponent,GravityBullet>());
		}

		if (m_timer >= fireRate) {
			m_timer = 0.0f;
		}

		m_prevState = m_state;

		if (stateTimer >= 5.0f) {
			ChangeState(State::Moving);
			stateTimer = 0.0f;
		}
	}

	else if (m_state == State::Moving) {

		if (!moveStates.empty()) {

			ShooterBehavior::SetDefaultPosition(transform.position);
			auto* playerb = BattleManager::GetCurrentCharacter();
			auto& playerTransform = playerb->GetEntity().GetComponent<ALC::Transform2D>();

			// target the players position by using this in velocity
			ALC::vec2 targetdir = playerTransform.position - transform.position;
			if (glm::length2(targetdir) > 0.0f) { targetdir = glm::normalize(targetdir); }
			ShooterBehavior::SetDefaultVelocity(targetdir * 300.0f);

			m_timer += ts.Get();

			if (m_timer <= ts.Get()) {

				ShooterBehavior::ShootRange(self, 7, bulRange , [tex](ALC::Entity bullet) {
					// update body collision
					auto& body = bullet.GetComponent<ALC::BulletBody>();
					body.radius = 4.0f;
					auto& sprite = bullet.GetComponent<ALC::SpriteComponent>();
					// this should be loaded ahead of time
					sprite.texture = tex;
					sprite.textureBounds = ALC::rect(16.0f, 80.0f, 31.0f, 95.0f);
					sprite.bounds = sprite.textureBounds.Centered();

				}, ALC::BulletTypes<BulletDeleterComponent, NormalBullet>());
			}


			ALC::uint8 tmpState = static_cast<ALC::uint8>(m_state);
			m_moveState.PerformMoveState(this, moveStates[0], &tmpState, ts, 0.0f, static_cast<ALC::uint8>(State::RainBullets));


			if (m_moveState.GetIsComplete()) {
				moveStates.erase(moveStates.begin());

				m_state = static_cast<State>(tmpState);
				m_timer = 0.0f;
			}

			if (m_timer >= 0.2f) {
				m_timer = 0.0f;
			}
		}

		if(moveStates.empty()){}
	}

}

void RuiEnemy::Phase1Begin(const Phase lastphase, ALC::Entity self, ALC::Timestep ts) {
	ResetHealth(500.0f);
	SetLifetime(500000.0f);

	if (m_state != State::None) {
		m_prevState = m_state;
	}
	 
	ResteTimer();

	if (!moveStates.empty()) { moveStates.clear(); }

	moveStates.push_back(MoveStates::States::Down);
	moveStates.push_back(MoveStates::States::DownRight);
	moveStates.push_back(MoveStates::States::Up);
	moveStates.push_back(MoveStates::States::Left);

	m_state = State::TwinSpin;
}
void RuiEnemy::Phase1Step(ALC::Entity self, ALC::Timestep ts) {

	// get our components
	auto [transform, cbody] = self.GetComponent<ALC::Transform2D, ALC::CharacterBody>();
	ALC::vec2 bultPos;
	auto tex = m_bulletTexture;

	if (m_state == State::TwinSpin) {
		int numOfBuillets = 150;
		stateTimer += ts.Get();

		if (GetHealth() >= GetMaxHealth()) {
			numOfBuillets = 100;
		}

		else if (GetHealth() <= (GetMaxHealth() / 2.0f)) {
			numOfBuillets = 200;
		}

		for (int i = 1; i < numOfBuillets; ++i) {

			m_timer += ts.Get();
			ALC::vec2 tmpVec;

			if (m_timer <= ts.Get()) {


				if (dirIndex < 3) {
					tmpVec = glm::rotate(((dir[dirIndex] + dir[dirIndex + 1]) / 2.0f) * 400.0f,
										 glm::radians(360.0f / static_cast<float>(numOfBuillets)) * static_cast<float>(i));

					bultPos = transform.position;

				}
				else if (dirIndex == 3) {
					tmpVec = glm::rotate(((dir[dirIndex] + dir[0]) / 2.0f) * 400.0f,
										 glm::radians(360.0f / static_cast<float>(numOfBuillets)) * static_cast<float>(i));

					bultPos = transform.position;
				}

				ShooterBehavior::SetDefaultPosition(bultPos);

				ShooterBehavior::Shoot(self, 1, [tmpVec, tex](ALC::Entity bullet) {
					// update body collision
					auto& body = bullet.GetComponent<ALC::BulletBody>();
					body.radius = 4.0f;
					body.velocity = tmpVec;

					auto& sprite = bullet.GetComponent<ALC::SpriteComponent>();
					//sprite.bounds = ALC::rect(14.0f);
					// this should be loaded ahead of time
					sprite.texture = tex;
					sprite.textureBounds = ALC::rect(16.0f, 80.0f, 31.0f, 95.0f);
					sprite.bounds = sprite.textureBounds.Centered();
				}, ALC::BulletTypes<BulletDeleterComponent,NormalBullet>());



				if (dirIndex < 3) {
					tmpVec = glm::rotate((((dir[dirIndex] + dir[dirIndex + 1]) / 2.0f) * -1.0f) * 400.0f,
										 glm::radians(360.0f / static_cast<float>(numOfBuillets)) * static_cast<float>(i));

					bultPos = transform.position;
				}
				else if (dirIndex == 3) {
					tmpVec = glm::rotate((((dir[dirIndex] + dir[0]) / 2.0f) * -1.0f) * 400.0f,
										 glm::radians(360.0f / static_cast<float>(numOfBuillets)) * static_cast<float>(i));

					bultPos = transform.position;
				}

				ShooterBehavior::SetDefaultPosition(transform.position);
				ShooterBehavior::SetDefaultVelocity(tmpVec);

				ShooterBehavior::Shoot(self, 1, [tex](ALC::Entity bullet) {
					// update body collision
					auto& body = bullet.GetComponent<ALC::BulletBody>();
					body.radius = 4.0f;

					auto& sprite = bullet.GetComponent<ALC::SpriteComponent>();
					//sprite.bounds = ALC::rect(14.0f);
					// this should be loaded ahead of time
					sprite.texture = tex;
					sprite.textureBounds = ALC::rect(16.0f, 80.0f, 31.0f, 95.0f);
					sprite.bounds = sprite.textureBounds.Centered();
				}, ALC::BulletTypes<BulletDeleterComponent,NormalBullet>());
			}

			if (m_timer >= 4.0f) {

				m_timer = 0.0f;
				dirIndex++;

				if (dirIndex > 3) {
					dirIndex = 0;
				}
			}
		}

		if (stateTimer >= 5.0f) {
			dirIndex = stateTimer = m_timer = 0.0f;
			m_prevState = m_state;
			m_state = State::Moving;
		}
	}

	else if (m_state == State::Moving) {
		 //check if list is empty 
		if (!moveStates.empty()) {

			ShooterBehavior::SetDefaultPosition(transform.position);

			m_timer += ts.Get();

			if (m_timer <= ts.Get()) {
				 //shoot a bullet when moving
				ShooterBehavior::ShootCircle(self, 40, [tex](ALC::Entity bullet) {
					// update body collision
					auto& body = bullet.GetComponent<ALC::BulletBody>();
					body.radius = 4.0f;
					auto& sprite = bullet.GetComponent<ALC::SpriteComponent>();
					// this should be loaded ahead of time
					sprite.texture = tex;
					sprite.textureBounds = ALC::rect(16.0f, 80.0f, 31.0f, 95.0f);
					sprite.bounds = sprite.textureBounds.Centered();
				}, ALC::BulletTypes<BulletDeleterComponent,NormalBullet>());
			}

			ALC::uint8 tmpState = static_cast<ALC::uint8>(m_state);
			m_moveState.PerformMoveState(this, moveStates[0], &tmpState, ts, 0.0f, static_cast<ALC::uint8>(State::TwinSpin));

			if (m_moveState.GetIsComplete()) { // when moving is complete  erease that moving state and go back to shooting state(just called state in ruiEnemy.cpp
				moveStates.erase(moveStates.begin());
				m_state = static_cast<State>(tmpState);
				ResteTimer();
			}

			if (m_timer >= .3f) {
				m_timer = 0.0f;
			}
		}
	}

	if (moveStates.empty()) {
		moveStates.push_back(MoveStates::States::Down);
		moveStates.push_back(MoveStates::States::DownRight);
		moveStates.push_back(MoveStates::States::Up);
		moveStates.push_back(MoveStates::States::Left);
	}
}

void RuiEnemy::Phase2Begin(const Phase lastphase, ALC::Entity self, ALC::Timestep ts) {

	ResetHealth(10.0f);
	SetLifetime(45.0f);

	if (m_state != State::None) {
		m_prevState = m_state;
	}

	if (!moveStates.empty()) { moveStates.clear(); }

	moveStates.push_back(MoveStates::States::DownLeft);
	moveStates.push_back(MoveStates::States::UpRight);
	moveStates.push_back(MoveStates::States::Left);
	moveStates.push_back(MoveStates::States::Move);

	m_state = State::CircleBombs;
}
void RuiEnemy::Phase2Step(ALC::Entity self, ALC::Timestep ts) {

	auto [transform, cbody] = self.GetComponent<ALC::Transform2D, ALC::CharacterBody>();
	auto tex = m_bulletTexture;
	bool shot = false;

	if (m_state == State::CircleBombs) {
		oldPos = transform.position;
		ShooterBehavior::SetDefaultPosition(transform.position);

		ShooterBehavior::SetDefaultVelocity(dir[0] * 300.0f);

		ShooterBehavior::ShootCircle(self, 60, [tex](ALC::Entity bullet) {
			// update body collision
			auto& body = bullet.GetComponent<ALC::BulletBody>();
			body.radius = 4.0f;
			auto& sprite = bullet.GetComponent<ALC::SpriteComponent>();
			// this should be loaded ahead of time
			sprite.texture = tex;
			sprite.textureBounds = ALC::rect(16.0f, 80.0f, 31.0f, 95.0f);
			sprite.bounds = sprite.textureBounds.Centered();
			}, ALC::BulletTypes<BulletDeleterComponent, CircleBomb>());

		shot = true;

		if (shot) {
			m_state = State::Moving;
		}
	}
	
	else if (m_state == State::Moving) {

		float fireRate = 0.4f;
		int numOfBullets = 50;

		if (GetHealth() < (GetMaxHealth() / 2)) { fireRate = 0.4f; numOfBullets = 65; }

		if (!moveStates.empty()) {

			ALC::uint8 tmpState = static_cast<ALC::uint8>(m_state);
			m_moveState.PerformMoveState(this, moveStates[0], &tmpState, ts, 0.0f, static_cast<ALC::uint8>(State::CircleBombs));
			
			m_timer += ts.Get();

			if (m_timer <= ts.Get()) {
				ShooterBehavior::SetDefaultPosition(oldPos);
				ShooterBehavior::ShootCircle(self, numOfBullets, [tex](ALC::Entity bullet) {
					// update body collision
					auto& body = bullet.GetComponent<ALC::BulletBody>();
					body.radius = 4.0f;
					auto& sprite = bullet.GetComponent<ALC::SpriteComponent>();
					// this should be loaded ahead of time
					sprite.texture = tex;
					sprite.textureBounds = ALC::rect(16.0f, 80.0f, 31.0f, 95.0f);
					sprite.bounds = sprite.textureBounds.Centered();
					}, ALC::BulletTypes<BulletDeleterComponent, CircleBomb>());

				ShooterBehavior::SetDefaultPosition(transform.position);
				ShooterBehavior::ShootCircle(self, numOfBullets, [tex](ALC::Entity bullet) {
					// update body collision
					auto& body = bullet.GetComponent<ALC::BulletBody>();
					body.radius = 4.0f;
					auto& sprite = bullet.GetComponent<ALC::SpriteComponent>();
					// this should be loaded ahead of time
					sprite.texture = tex;
					sprite.textureBounds = ALC::rect(16.0f, 80.0f, 31.0f, 95.0f);
					sprite.bounds = sprite.textureBounds.Centered();
					}, ALC::BulletTypes<BulletDeleterComponent, NormalBullet>());
			}

			if (m_moveState.GetIsComplete()) {
				moveStates.push_back(moveStates[0]);
				moveStates.erase(moveStates.begin());
				m_state = static_cast<State>(tmpState);
				m_timer = 0.0f;
			}

			if (m_timer >= fireRate) {
				m_timer = 0.0f;
			}
		}
	}
}

void RuiEnemy::Phase3Begin(const Phase lastphase, ALC::Entity self, ALC::Timestep ts) {
	ResetHealth(1000.0f);
	SetLifetime(5000000.0f);

	if (m_state != State::None) {
		m_prevState = m_state;
	}

	if (!moveStates.empty()) { moveStates.clear(); }

	moveStates.push_back(MoveStates::States::UpLeft);
	moveStates.push_back(MoveStates::States::UpRight);
	moveStates.push_back(MoveStates::States::Left);
	moveStates.push_back(MoveStates::States::Move);
	moveStates.push_back(MoveStates::States::DownLeft);
	moveStates.push_back(MoveStates::States::Right);

	//m_state = State::RainBullets;
}
void RuiEnemy::Phase3Step(ALC::Entity self, ALC::Timestep ts) {
	auto [transform, cbody] = self.GetComponent<ALC::Transform2D, ALC::CharacterBody>();
	auto tex = m_bulletTexture;

	auto* playerb = BattleManager::GetCurrentCharacter();
	auto& playerTransform = playerb->GetEntity().GetComponent<ALC::Transform2D>();

	// target the players position by using this in velocity
	ALC::vec2 targetdir = playerTransform.position - transform.position;
	if (glm::length2(targetdir) > 0.0f) { targetdir = glm::normalize(targetdir); }


	ShooterBehavior::SetDefaultVelocity(dir[2] * 300.0f);

	m_timer += ts.Get();
	stateTimer += ts.Get();

	if (m_timer <= ts.Get()) {
		ShooterBehavior::SetDefaultPosition(ALC::vec2(0.0f, BattleManager::GetLevelBounds().top));
		ShooterBehavior::Shoot(self, 1, [tex](ALC::Entity bullet) {
			// update body collision
			auto& body = bullet.GetComponent<ALC::BulletBody>();
			body.radius = 4.0f;
			auto& sprite = bullet.GetComponent<ALC::SpriteComponent>();
			// this should be loaded ahead of time
			sprite.texture = tex;
			sprite.textureBounds = ALC::rect(16.0f, 80.0f, 31.0f, 95.0f);
			sprite.bounds = sprite.textureBounds.Centered();
			}, ALC::BulletTypes<BulletDeleterComponent, ZigZagBullets>());

		ShooterBehavior::SetDefaultPosition(ALC::vec2(BattleManager::GetLevelBounds().left + 10.0f, BattleManager::GetLevelBounds().top));
		ShooterBehavior::Shoot(self, 1, [tex](ALC::Entity bullet) {
			// update body collision
			auto& body = bullet.GetComponent<ALC::BulletBody>();
			body.radius = 4.0f;
			auto& sprite = bullet.GetComponent<ALC::SpriteComponent>();
			// this should be loaded ahead of time
			sprite.texture = tex;
			sprite.textureBounds = ALC::rect(16.0f, 80.0f, 31.0f, 95.0f);
			sprite.bounds = sprite.textureBounds.Centered();
			}, ALC::BulletTypes<BulletDeleterComponent, ZigZagBullets>());

		ShooterBehavior::SetDefaultPosition(ALC::vec2(BattleManager::GetLevelBounds().right - 10.0f, BattleManager::GetLevelBounds().top));
		ShooterBehavior::Shoot(self, 1, [tex](ALC::Entity bullet) {
			// update body collision
			auto& body = bullet.GetComponent<ALC::BulletBody>();
			body.radius = 4.0f;
			auto& sprite = bullet.GetComponent<ALC::SpriteComponent>();
			// this should be loaded ahead of time
			sprite.texture = tex;
			sprite.textureBounds = ALC::rect(16.0f, 80.0f, 31.0f, 95.0f);
			sprite.bounds = sprite.textureBounds.Centered();
			}, ALC::BulletTypes<BulletDeleterComponent, ZigZagBullets>());

		ShooterBehavior::SetDefaultPosition(ALC::vec2(BattleManager::GetLevelBounds().left / 2.0f, BattleManager::GetLevelBounds().top));
		ShooterBehavior::Shoot(self, 1, [tex](ALC::Entity bullet) {
			// update body collision
			auto& body = bullet.GetComponent<ALC::BulletBody>();
			body.radius = 4.0f;
			auto& sprite = bullet.GetComponent<ALC::SpriteComponent>();
			// this should be loaded ahead of time
			sprite.texture = tex;
			sprite.textureBounds = ALC::rect(16.0f, 80.0f, 31.0f, 95.0f);
			sprite.bounds = sprite.textureBounds.Centered();
			}, ALC::BulletTypes<BulletDeleterComponent, ZigZagBullets>());

		ShooterBehavior::SetDefaultPosition(ALC::vec2(BattleManager::GetLevelBounds().right / 2.0f, BattleManager::GetLevelBounds().top));
		ShooterBehavior::Shoot(self, 1, [tex](ALC::Entity bullet) {
			// update body collision
			auto& body = bullet.GetComponent<ALC::BulletBody>();
			body.radius = 4.0f;
			auto& sprite = bullet.GetComponent<ALC::SpriteComponent>();
			// this should be loaded ahead of time
			sprite.texture = tex;
			sprite.textureBounds = ALC::rect(16.0f, 80.0f, 31.0f, 95.0f);
			sprite.bounds = sprite.textureBounds.Centered();
			}, ALC::BulletTypes<BulletDeleterComponent, ZigZagBullets>());
	}

	ShooterBehavior::SetDefaultPosition(transform.position);
	ShooterBehavior::SetDefaultVelocity(targetdir * 300.0f);

	if (m_timer <= ts.Get()) {
		//shoot a bullet when moving
		ShooterBehavior::ShootRange(self, 5, 45.0f, [tex](ALC::Entity bullet) {
			// update body collision
			auto& body = bullet.GetComponent<ALC::BulletBody>();
			body.radius = 4.0f;
			auto& sprite = bullet.GetComponent<ALC::SpriteComponent>();
			// this should be loaded ahead of time
			sprite.texture = tex;
			sprite.textureBounds = ALC::rect(16.0f, 80.0f, 31.0f, 95.0f);
			sprite.bounds = sprite.textureBounds.Centered();
			}, ALC::BulletTypes<BulletDeleterComponent, NormalBullet>());
	}

	if (!moveStates.empty()) {
		ALC::uint8 tmpState = static_cast<ALC::uint8>(m_state);
		m_moveState.PerformMoveState(this, moveStates[0], &tmpState, ts, 0.0f);

		if (m_moveState.GetIsComplete()) { // when moving is complete  erease that moving state and go back to shooting state(just called state in ruiEnemy.cpp
			moveStates.erase(moveStates.begin());
			ResteTimer();
		}
	}
	else {

	}

	if(m_timer >= .3f) {
		m_timer = 0.0f;
	}
	
}

void RuiEnemy::Phase4Begin(const Phase lastphase, ALC::Entity self, ALC::Timestep ts) {
	ResetHealth(2500.0f);
	SetLifetime(5000000.0f);

	m_timer = 0.0f;

	if (m_state != State::None) { m_prevState = m_state; }

	if (!moveStates.empty()) { moveStates.clear(); }

	moveStates.push_back(MoveStates::States::DownLeft);
	moveStates.push_back(MoveStates::States::UpRight);
	moveStates.push_back(MoveStates::States::Left);
	moveStates.push_back(MoveStates::States::Right);
	moveStates.push_back(MoveStates::States::DownRight);
	moveStates.push_back(MoveStates::States::Move);
}
void RuiEnemy::Phase4Step(ALC::Entity self, ALC::Timestep ts) { 

	auto [transform, cbody] = self.GetComponent<ALC::Transform2D, ALC::CharacterBody>();
	auto tex = m_bulletTexture;

	float rndRange = rand() % 360 + 25;
	float rndBulAmount = rand() % 5 + 1;
	float rndVelScal = rand() % 100 + 75;
	int rndDir = rand() % 4;

	//returns a random fire rate between 0.0 and 1.0 and then divides it by 2 
	float rndFireRate = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) / 2.0f;

	// get a x pos between the left and right bounds of the level 
	float rndXPos = static_cast<int>(BattleManager::GetLevelBounds().left) + rand() % (static_cast<int>(BattleManager::GetLevelBounds().right) 
					- static_cast<int>(BattleManager::GetLevelBounds().left) + 1);

	// get a y pos for the top to the enemies bottom (top / 4) of the level   
	float rndYPos = rand() % static_cast<int>(BattleManager::GetLevelBounds().top) + static_cast<int>(BattleManager::GetLevelBounds().top / 4);

	ShooterBehavior::SetDefaultPosition(ALC::vec2(rndXPos,rndYPos));
	ShooterBehavior::SetDefaultVelocity(dir[rndDir] * rndVelScal);

	m_timer += ts.Get();
	m_secondTimer += ts.Get();

	if (m_timer <= ts.Get()) {

		ShooterBehavior::ShootRange(self, rndBulAmount,rndRange, [tex](ALC::Entity bullet) {
			// update body collision
			auto& body = bullet.GetComponent<ALC::BulletBody>();
			body.radius = 4.0f;
			auto& sprite = bullet.GetComponent<ALC::SpriteComponent>();
			// this should be loaded ahead of time
			sprite.texture = tex;
			sprite.textureBounds = ALC::rect(16.0f, 80.0f, 31.0f, 95.0f);
			sprite.bounds = sprite.textureBounds.Centered();
		}, ALC::BulletTypes<BulletDeleterComponent, NormalBullet>());
	}

	ALC::uint8 tmpState = static_cast<ALC::uint8>(m_state);
	m_moveState.PerformMoveState(this, moveStates[0], &tmpState, ts, 0.0f);

	if (m_moveState.GetIsComplete()) { // when moving is complete  erease that moving state and go back to shooting state(just called state in ruiEnemy.cpp
		moveStates.erase(moveStates.begin());
		ResteTimer();
	}

	if (moveStates.empty()) {
		ALC::uint8 rndPhase = rand() % 5;
		m_phases.ChangeState(static_cast<Phase>(rndPhase));
	}

	if (m_timer >= rndFireRate ) {
		m_timer = 0.0f;
	}
}

void RuiEnemy::PostBattleBegin(const Phase lastphase, ALC::Entity self, ALC::Timestep ts) {
	// enemy is done fighting
	MarkDone();
}
void RuiEnemy::PostBattleStep(ALC::Entity self, ALC::Timestep ts) { }

void RuiEnemy::ChangeDirectionState(State curntState)
{
	State randomState = static_cast<State>(rand() % 11 + 1);

	if (randomState == curntState) {
		ChangeDirectionState(curntState);
	}

	if (m_state != State::None) {
		m_prevState = m_state;
	}

	m_state = randomState;
}

void RuiEnemy::ChangeState(State nextState)
{
	if (m_state != State::None) {
		m_prevState = m_state;
	}

	m_state = nextState;
}


