#include "RuiEnemy.hpp"
#include "../Systems/BulletDeleterSystem.hpp"
#include "../Systems/GravityBulletSystem.hpp"
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
	SetAccel(50 * 30);
	SetSpeed(100 *20);
	m_bulletTexture = ALC::ContentManager::LoadTexture("Resources/Textures/Spells.png");

	auto& spr = self.GetComponent<ALC::SpriteComponent>();
	spr.texture = ALC::ContentManager::LoadTexture("Resources/Textures/Enemies/Rui-Enemy.png");
	spr.textureBounds = spr.texture.GetBounds();
	spr.bounds = spr.texture.GetCenteredBounds();

	// match the ratio 
	//float ratio = spr.textureBounds.Height() / spr.textureBounds.Width();
	//float newHeight = spr.bounds.Width() * ratio;
	//spr.bounds.top = newHeight * 0.5f;
	//spr.bounds.bottom = -newHeight * 0.5f;

}

void RuiEnemy::Update(ALC::Entity self, ALC::Timestep ts) {
	// update the phase
	m_phases(self, ts);
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
}

void RuiEnemy::BattleBegin() {
	// we start by changing phases
	m_phases.ChangeState(Phase::Phase0);
}

void RuiEnemy::PreBattleBegin(const Phase lastphase, ALC::Entity self, ALC::Timestep ts) { }
void RuiEnemy::PreBattleStep(ALC::Entity self, ALC::Timestep ts) { }

void RuiEnemy::Phase0Begin(const Phase lastphase, ALC::Entity self, ALC::Timestep ts) {
	ResetHealth(100.0f);
	SetLifetime(300000.0f);

	if (m_state != State::None) {
		m_prevState = m_state;
	}

	if (!moveStates.empty()) { moveStates.clear(); }

	moveStates.push_back(MoveStates::States::Up);
	moveStates.push_back(MoveStates::States::DownLeft);
	moveStates.push_back(MoveStates::States::UpRight);
	moveStates.push_back(MoveStates::States::Left);
	moveStates.push_back(MoveStates::States::Move);



	m_state = State::Shoot3;
	// make sure our bullets spawn with this component
	ShooterBehavior::SetBulletTypes<BulletDeleterComponent,GravityBullet>();
}
void RuiEnemy::Phase0Step(ALC::Entity self, ALC::Timestep ts) {

	ALC::vector<ALC::Entity> bullets;


	auto [transform, cbody] = self.GetComponent<ALC::Transform2D, ALC::CharacterBody>();

	ALC::vec2 pos = transform.position - 8.0f;
	float speed = 10.0f;

	auto* playerb = BattleManager::GetCurrentCharacter();
	auto& playerTransform = playerb->GetEntity().GetComponent<ALC::Transform2D>();
	
	// target the players position by using this in velocity
	ALC::vec2 targetdir = playerTransform.position - transform.position;
	if (glm::length2(targetdir) > 0.0f) { targetdir = glm::normalize(targetdir); }

	if (m_state == State::Shoot3) {
		// set value
		ShooterBehavior::SetDefaultPosition(transform.position);
		
		ShooterBehavior::SetDefaultVelocity(dir[0] * 300.0f);

		auto tex = m_bulletTexture;
		m_timer += ts.Get();
		if (m_timer <= ts.Get()) {
			
		// uses default velocity to shoot a range
		// if default is [0, 1] then it will shoot in a range from (-45.0f / 2) to (+45.0f / 2)
		ShooterBehavior::ShootRange(self, 3, 45.0f, [tex](ALC::Entity bullet) {
			// update body collision
			auto& body = bullet.GetComponent<ALC::BulletBody>();
			body.radius = 4.0f;

			auto& sprite = bullet.GetComponent<ALC::SpriteComponent>();
			// this should be loaded ahead of time
			sprite.texture = tex;
			sprite.textureBounds = ALC::rect(16.0f, 80.0f, 31.0f, 95.0f);
			sprite.bounds = sprite.textureBounds.Centered();
		}, ALC::BulletTypes<BulletDeleterComponent, NormalBullet>());

		// change state
		m_state = State::Move;
	} else if (m_state == State::Move) {

		// move
		auto result = BTA::MoveTo(&cbody.velocity, transform.position, ALC::vec2(-100, 100), 400.0f, speed * 20.0f, ts);

		if (result == BTA::Result::Arrived) {
			// set value
			ShooterBehavior::SetDefaultPosition(transform.position);
			ShooterBehavior::SetDefaultVelocity(targetdir * 300.0f);

			auto tex = m_bulletTexture;

			ShooterBehavior::ShootRange(self, 10, 45.0f, [tex](ALC::Entity bullet) {
				// update body collision
				auto& body = bullet.GetComponent<ALC::BulletBody>();
				body.radius = 4.0f;
				auto& sprite = bullet.GetComponent<ALC::SpriteComponent>();
				// this should be loaded ahead of time
				sprite.texture = tex;
				sprite.textureBounds = ALC::rect(16.0f, 80.0f, 31.0f, 95.0f);
				sprite.bounds = sprite.textureBounds.Centered();
				
			});
		}
			}, ALC::BulletTypes<BulletDeleterComponent, NormalBullet>());

		if (m_timer >= .2f) {
			m_timer = 0.0f;
		}

		m_prevState = m_state;
		
	} 
	
}

void RuiEnemy::Phase1Begin(const Phase lastphase, ALC::Entity self, ALC::Timestep ts) {
	ResetHealth(1000.0f);
	SetLifetime(1000000000000.0f);

	if (m_state != State::None) {
		m_prevState = m_state;
	}

	if (!moveStates.empty()) {
		moveStates.clear();
	}


	moveStates.push_back(MoveStates::States::Down);
	moveStates.push_back(MoveStates::States::DownRight);
	moveStates.push_back(MoveStates::States::Up);
	moveStates.push_back(MoveStates::States::Left);

	m_state = State::TwinSpin;
	// make sure our bullets spawn with this component
	ShooterBehavior::SetBulletTypes<BulletDeleterComponent>();

}

void RuiEnemy::Phase1Step(ALC::Entity self, ALC::Timestep ts) {

	// get our components
	auto [transform, cbody] = self.GetComponent<ALC::Transform2D, ALC::CharacterBody>();
	ALC::vec2 bultPos;
	auto lvlBounds = BattleManager::GetLevelBounds();
	float speed = 10.0f;
	auto arved = BTA::Result::Arrived;
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

			if (dirIndex < 3) {
				tmpVec = glm::rotate(((dir[dirIndex] + dir[dirIndex + 1]) / 2.0f) * 400.0f,

									 glm::radians(360.0f / static_cast<float>(numOfBuillets)) * static_cast<float>(i));
			} else if (dirIndex == 3) {
				tmpVec = glm::rotate(((dir[dirIndex] + dir[0]) / 2.0f) * 400.0f,
									 glm::radians(360.0f / static_cast<float>(numOfBuillets)) * static_cast<float>(i));
			}

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
					});

					

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
					});
			}

			if (m_timer >= 4.0f) {

				m_timer = 0.0f;
				dirIndex++;

				if (dirIndex > 3) {
					dirIndex = 0;
				}
				auto& sprite = bullet.GetComponent<ALC::SpriteComponent>();
				// this should be loaded ahead of time
				sprite.texture = tex;
				sprite.textureBounds = ALC::rect(16.0f, 80.0f, 31.0f, 95.0f);
				sprite.bounds = sprite.textureBounds.Centered();
			}, ALC::BulletTypes<BulletDeleterComponent, NormalBullet>());

			if (dirIndex < 3) {
				tmpVec = glm::rotate(((dir[dirIndex] + dir[dirIndex + 1]) / (2.0f * -1.0f)) * 400.0f,
									 glm::radians(360.0f / static_cast<float>(numOfBuillets)) * static_cast<float>(i));
			} else if (dirIndex == 3) {
				tmpVec = glm::rotate(((dir[dirIndex] + dir[0]) / (2.0f * -1.0f)) * 400.0f,
									 glm::radians(360.0f / static_cast<float>(numOfBuillets)) * static_cast<float>(i));
			}
		}

		if (stateTimer >= 5.0f) {
			dirIndex = stateTimer = m_timer = 0.0f;
			m_prevState = m_state;
			m_state = State::Moving;
		}
	}

	else if (m_state == State::Moving) {

		if (!moveStates.empty()) {

			ShooterBehavior::SetDefaultPosition(transform.position);

			m_timer += ts.Get();

			if (m_timer <= ts.Get()) {
				ShooterBehavior::ShootCircle(self, 30, [tex](ALC::Entity bullet) {
					// update body collision
					auto& body = bullet.GetComponent<ALC::BulletBody>();
					body.radius = 4.0f;
					auto& sprite = bullet.GetComponent<ALC::SpriteComponent>();
					body.isSimulated = false;
					// this should be loaded ahead of time
					sprite.texture = tex;
					sprite.textureBounds = ALC::rect(16.0f, 80.0f, 31.0f, 95.0f);
					sprite.bounds = sprite.textureBounds.Centered();
				});
					
			}
				
				auto& sprite = bullet.GetComponent<ALC::SpriteComponent>();
				// this should be loaded ahead of time
				sprite.texture = tex;
				sprite.textureBounds = ALC::rect(16.0f, 80.0f, 31.0f, 95.0f);
				sprite.bounds = sprite.textureBounds.Centered();
			}, ALC::BulletTypes<BulletDeleterComponent, NormalBullet>());


			ALC::uint8 tmpState = static_cast<ALC::uint8>(m_state);
			m_moveState.PerformMoveState(this, moveStates[0], &tmpState, ts, 0.0f, static_cast<ALC::uint8>(State::TwinSpin));
			

			if (m_moveState.GetIsComplete()) {
				moveStates.erase(moveStates.begin());
		}

				ShooterBehavior::ShootCircle(self, 20, [tex](ALC::Entity bullet) {
					// update body collision
					auto& body = bullet.GetComponent<ALC::BulletBody>();
					body.radius = 4.0f;

					auto& sprite = bullet.GetComponent<ALC::SpriteComponent>();
					//sprite.bounds = ALC::rect(14.0f);
					// this should be loaded ahead of time
					sprite.texture = tex;
					sprite.textureBounds = ALC::rect(16.0f, 80.0f, 31.0f, 95.0f);
					sprite.bounds = sprite.textureBounds.Centered();
				});

				m_state = static_cast<State>(tmpState);
				m_timer = 0.0f;
			}

			if (m_timer >= .5f) {
				m_timer = 0.0f;
			}
		}
	}

		else {
			m_phases.ChangeState(Phase::Phase2);
		}
	}
}

void RuiEnemy::Phase2Begin(const Phase lastphase, ALC::Entity self, ALC::Timestep ts) { }
void RuiEnemy::Phase2Step(ALC::Entity self, ALC::Timestep ts) { }

void RuiEnemy::Phase3Begin(const Phase lastphase, ALC::Entity self, ALC::Timestep ts) { }
void RuiEnemy::Phase3Step(ALC::Entity self, ALC::Timestep ts) { }

void RuiEnemy::Phase4Begin(const Phase lastphase, ALC::Entity self, ALC::Timestep ts) { }
void RuiEnemy::Phase4Step(ALC::Entity self, ALC::Timestep ts) { }

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


