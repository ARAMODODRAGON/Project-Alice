#include "AliceChara.hpp"
#include <glm\gtx\rotate_vector.hpp>
#include "../Systems/BulletDeleterSystem.hpp"
#include "../Systems/Bullet Types/HomingBulletSystem.hpp"
#include "../BTA.hpp"

#define VEC3_DOWN ALC::vec3(0.0f, -1.0f, 0.0f)
#define VEC2_DOWN ALC::vec2(0.0f, -1.0f)

AliceChara::AliceChara()
	: m_basicShootTimer(0.0f)
	, m_homingShootTimer(0.0f)
	, m_basicShootSpeed(0.04f)
	, m_homingShootSpeed(0.12f)
	, m_rotationspeed(300.0f)
	, m_spinspeed(900.0f)
	, m_isRepositioning(false)
	, m_activeSpell(this, State::Homing)
	, m_lastSpell(State::Homing)
	, m_shieldCharge(100.0f)
	, m_shieldChargeRate(2.0f)
	, m_spriteAnimationTimer(0.0f)
	, m_spriteAnimationSpeed(0.1f)
	, m_curSpriteAnimation(0) {
	// bind states
	m_activeSpell.Bind(State::Homing, &AliceChara::StateStepHoming, &AliceChara::StateBeginHoming);
	m_activeSpell.Bind(State::Spinning, &AliceChara::StateStepSpinning, &AliceChara::StateBeginSpinning);
	m_activeSpell.Bind(State::Shield, &AliceChara::StateStepShield, &AliceChara::StateBeginShield, &AliceChara::StateEndShield);
	//m_activeSpell.Bind(State::Death, &AliceChara::StateStepDeath, &AliceChara::StateBeginDeath);
}

AliceChara::~AliceChara() { }

void AliceChara::Start(ALC::Entity self) {
	// call base
	Character::Start(self);
	using CM = ALC::ContentManager;

	// set character texture
	auto& selfSpr = self.GetComponent<ALC::SpriteComponent>();
	selfSpr.texture = CM::LoadTexture("Resources/Textures/Characters/Alice-Character-Sheet.png");
	selfSpr.textureBounds = ALC::rect(0.0f, 0.0f, 47.0f, 79.0f);
	selfSpr.bounds = selfSpr.textureBounds.Centered();
	// get textures
	m_spellsTexture = CM::LoadTexture("Resources/Textures/Spells.png");

	// create our point shooters
	for (size_t i = 0; i < m_pointShooters.size(); i++) {
		auto& point = m_pointShooters[i];

		// create the entity
		auto& reg = GetRegistry();
		auto entity = reg.Create();

		// get the ID
		point.entityID = entity.GetComponent<ALC::EntityInfo>().GetID();

		// add the sprite component & transform
		auto& spr = entity.AddComponent<ALC::SpriteComponent>();
		auto& tr = entity.AddComponent<ALC::Transform2D>();

		// get player transform
		auto& selftr = self.GetComponent<ALC::Transform2D>();

		// set relative position;
		if (i == 0) point.rotation = 90.0f; // one quart
		else		point.rotation = 270.0f; // three quarts
		point.distance = 33.0f;

		// set position
		tr.position = point.CalcPosition(selftr.position);

		// set the sprite stuff
		spr.texture = m_spellsTexture;
		spr.layer = SPRL::PLAYER_COLLIDER;

		if (i == 0) {
			spr.textureBounds = ALC::rect(ALC::vec2(16.0f, 32.0f), ALC::vec2(32.0f, 48.0f));
		} else {
			spr.textureBounds = ALC::rect(ALC::vec2(0.0f, 48.0f), ALC::vec2(15.0f, 63.0f));
		}
		spr.bounds = spr.textureBounds.Centered();

	}

	// create the shield sprite object
	{
		auto shield = GetRegistry().Create();
		m_shieldEntity = shield.GetComponent<ALC::EntityInfo>().GetID();

		auto& transform = shield.AddComponent<ALC::Transform2D>();
		auto& sprite = shield.AddComponent<ALC::SpriteComponent>();

		sprite.texture = m_spellsTexture;
		sprite.textureBounds = ALC::rect(32.0f, 0.0f, 111.0f, 79.0f);
		sprite.bounds = sprite.textureBounds.Centered();
		sprite.shouldDraw = false;
		sprite.layer = SPRL::PLAYER_SHIELD;
	}

	// create death projectiles
	for (size_t i = 0; i < m_deathProjectiles.size(); ++i) {
		auto& deathp = m_deathProjectiles[i];

		auto entity = GetRegistry().Create();
		deathp.entityID = entity.GetComponent<ALC::EntityInfo>().GetID();
		deathp.velocity = glm::rotate(VEC2_DOWN, glm::radians(float(i) * 45.0f)) * 500.0f;

		auto& sprite = entity.AddComponent<ALC::SpriteComponent>();
		auto& transform = entity.AddComponent<ALC::Transform2D>();

		sprite.texture = m_spellsTexture;
		sprite.layer = SPRL::PLAYER_SHIELD;
		sprite.shouldDraw = false;

		if (i % 2)	sprite.textureBounds = ALC::rect(0.0f, 128.0f, 15.0f, 143.0f);
		else		sprite.textureBounds = ALC::rect(0.0f, 144.0f, 15.0f, 159.0f);
		sprite.bounds = sprite.textureBounds.Centered();

	}
}

void AliceChara::Update(ALC::Entity self, ALC::Timestep ts) {

	if (ALC::Keyboard::GetKey(ALC::KeyCode::KeyF).Pressed())
		TakeDamage();

	// is not dead
	if (!IsDead()) {
		UpdateMovement(self, ts);

		// toggle state
		if (GetModButton().Pressed()) {
			switch (m_activeSpell.GetState()) {
				case State::Homing:
					m_activeSpell.ChangeState(State::Spinning);
					break;
				case State::Spinning:
					m_activeSpell.ChangeState(State::Homing);
					break;
				default: ALC_DEBUG_WARNING("failed to switch states"); break;
			}
		}

		// update charge
		if (m_activeSpell.GetState() != State::Shield) {
			// update and then check if the player activated it
			m_shieldCharge += m_shieldChargeRate * ts;
			if (m_shieldCharge >= 100.0f) {
				m_shieldCharge = 100.0f; // Caps at 100.0f otherwise weird things happen with the cooldown texture and it's just a mess lmao
				if (GetBurstButton().Pressed()) {
					m_shieldCharge = 0.0f;

					// switch states
					m_lastSpell = m_activeSpell.GetState();
					m_activeSpell.ChangeState(State::Shield);
				}
			}
		}

		// update state
		m_activeSpell(self, ts);

		auto& selftr = self.GetComponent<ALC::Transform2D>();
		auto& reg = GetRegistry();
		for (auto& point : m_pointShooters) {
			auto entity = reg.GetEntity(point.entityID);
			if (!entity) {
				ALC_DEBUG_WARNING("point shooter entity is missing");
				continue;
			}
			auto& tr = entity.GetComponent<ALC::Transform2D>();
			tr.position = point.CalcPosition(selftr.position);
		}

	}

	// is dead
	else {
		for (size_t i = 0; i < m_deathProjectiles.size(); i++) {
			auto& deathp = m_deathProjectiles[i];
			auto entity = GetRegistry().GetEntity(deathp.entityID);
			auto& transform = entity.GetComponent<ALC::Transform2D>();
			auto& sprite = entity.GetComponent<ALC::SpriteComponent>();

			// slowly become invisible
			sprite.color.a *= 0.9f;

			// move but also slow down overtime
			transform.position += (deathp.velocity *= 0.9f) * ts;

			// stop
			if (ALC::NearlyZero(deathp.velocity, (0.1f * 500.0f) * ts)) {
				sprite.shouldDraw = false;
				deathp.velocity = ALC::vec2(0.0f);
			}
		}
	}
}

void AliceChara::OnDeath(ALC::Entity self) {
	auto [selftr, selfspr] = self.GetComponent<ALC::Transform2D, ALC::SpriteComponent>();

	// hide player
	selfspr.shouldDraw = false;

	// hide collider
	auto& colspr = GetColliderSprite().GetComponent<ALC::SpriteComponent>();
	colspr.shouldDraw = false;

	// show the death projectiles
	for (size_t i = 0; i < m_deathProjectiles.size(); i++) {
		auto& deathp = m_deathProjectiles[i];
		auto entity = GetRegistry().GetEntity(deathp.entityID);

		auto& sprite = entity.GetComponent<ALC::SpriteComponent>();
		sprite.shouldDraw = true;

		auto& transform = entity.GetComponent<ALC::Transform2D>();
		transform.position = selftr.position;
	}

	// hide point shooters
	for (auto& point : m_pointShooters) {
		auto entity = GetRegistry().GetEntity(point.entityID);
		auto& sprite = entity.GetComponent<ALC::SpriteComponent>();
		sprite.shouldDraw = false;
	}
}

void AliceChara::LateUpdate(ALC::Entity self, ALC::Timestep ts) {
	UpdateCollisions(self, ts);
	UpdateSprites(self, ts);

	m_spriteAnimationTimer += ts;
	while (m_spriteAnimationTimer > m_spriteAnimationSpeed) {
		m_spriteAnimationTimer -= m_spriteAnimationSpeed;
		m_curSpriteAnimation = (m_curSpriteAnimation + 1) % 2;

		auto& selfSpr = self.GetComponent<ALC::SpriteComponent>();
		if (m_curSpriteAnimation == 0)	selfSpr.textureBounds = ALC::rect(0.0f, 0.0f, 47.0f, 79.0f);
		else							selfSpr.textureBounds = ALC::rect(48.0f, 0.0f, 95.0f, 79.0f);
	}
}

ALC::rect AliceChara::GetAttackTargetRect() const {
	State s = (m_activeSpell.GetState() == State::Shield ? m_lastSpell : m_activeSpell.GetState());
	switch (s) {
		case State::Homing: return ALC::rect(68.0f, 4.0f, 92.0f, 28.0f);
		case State::Spinning: return ALC::rect(36.0f, 4.0f, 60.0f, 28.0f);
		default: break;
	}
	return ALC::rect(4.0f, 4.0f, 28.0f, 28.0f);
}

ALC::rect AliceChara::GetDefenceTargetRect() const {
	return ALC::rect(4.0f, 36.0f, 28.0f, 60.0f);
}

ALC::rect AliceChara::GetDefenceTargetRectCooldown() const {
	return ALC::rect(4.0f, 60.0f - (24.0f * m_shieldCharge / 100.0f), 28.0f, 60.0f);
}

ALC::rect AliceChara::GetHeartTargetRect() const {
	if (IsInvuln()) return ALC::rect(97.0f, 7.0f, 106.0f, 15.0f);
	else			return ALC::rect(97.0f, 18.0f, 106.0f, 26.0f);
}

float AliceChara::RotateTowards(float curangle, const float target, const float speed) {
	if (ALC::NearlyEqual(curangle, target, speed))
		curangle = target;
	else if (curangle > target)
		curangle -= speed;
	else if (curangle < target)
		curangle += speed;
	return curangle;
}

#pragma region Homing State

void AliceChara::StateBeginHoming(const State laststate, ALC::Entity self, ALC::Timestep ts) {
	m_isRepositioning = false;
	for (size_t i = 0; i < m_pointShooters.size(); i++) {
		auto& point = m_pointShooters[i];
		const float targetrot = (i == 0 ? 90.0f : 270.0f);
		if (!ALC::NearlyEqual(point.rotation, targetrot))
			m_isRepositioning = true;
	}
}
void AliceChara::StateStepHoming(ALC::Entity self, ALC::Timestep ts) {
	// make sure the points are in the right position
	if (m_isRepositioning) {
		float speed = m_spinspeed * 3.0f;
		// tracks if its done repositioning
		bool confirm = true;

		// reposition
		for (size_t i = 0; i < m_pointShooters.size(); i++) {
			auto& point = m_pointShooters[i];
			const float targetrot = (i == 0 ? 90.0f : 270.0f);
			point.rotation = RotateTowards(point.rotation, targetrot, speed * ts);
			// done repose
			if (ALC::NearlyEqual(point.rotation, targetrot, speed * ts))
				point.rotation = targetrot;
			// not done repos
			else confirm = false;
		}

		if (confirm)	m_isRepositioning = false;
		else			return;
	}

	float slowmult = (GetSlowButton() ? 1.13f : 1.0f);

	bool basicShoot = false;
	float delay = 0.0f; // account for when the timer > shoottime and so the bullet needs to spawn earlier

	// basic shooting
	if (GetShootButton())	m_basicShootTimer += ts * slowmult;
	else					m_basicShootTimer = 0.0f;
	while (m_basicShootTimer > m_basicShootSpeed) {
		m_basicShootTimer -= m_basicShootSpeed;
		delay = m_basicShootTimer;
		basicShoot = true;
		ShooterBehavior::SetDefaultVelocity(ALC::vec2(0.0f, 800.0f) * slowmult);
	}

	// get self position
	auto& reg = GetRegistry();

	for (size_t i = 0; i < m_pointShooters.size(); i++) {
		// relevant info
		auto& point = m_pointShooters[i];
		auto entity = reg.GetEntity(point.entityID);
		auto& tr = entity.GetComponent<ALC::Transform2D>();

		// confirm they are at the right position;
		if (!GetSlowButton()) {
			if (i == 0)
				point.rotation = RotateTowards(point.rotation, 90.0f, m_rotationspeed * ts);
			else
				point.rotation = RotateTowards(point.rotation, 270.0f, m_rotationspeed * ts);
		} else {
			if (i == 0)
				point.rotation = RotateTowards(point.rotation, 150.0f, m_rotationspeed * ts);
			else
				point.rotation = RotateTowards(point.rotation, 210.0f, m_rotationspeed * ts);
		}

		// shoot
		if (basicShoot) {
			auto tex = m_spellsTexture;
			ShooterBehavior::SetDefaultPosition(tr.position);
			ShooterBehavior::Shoot(self, 1, [i, delay, tex](ALC::Entity e) {
				// update body collision
				auto& body = e.GetComponent<ALC::BulletBody>();
				body.radius = 18.0f;

				// update position
				auto& tr = e.GetComponent<ALC::Transform2D>();
				tr.position += body.velocity * delay;

				// update sprite
				auto& spr = e.GetComponent<ALC::SpriteComponent>();
				spr.color.a = 0.3f;
				spr.texture = tex;

				if (i == 0) spr.textureBounds = ALC::rect(0.0f, 16.0f, 15.0f, 31.0f);
				else		spr.textureBounds = ALC::rect(16.0f, 16.0f, 31.0f, 31.0f);
				spr.bounds = spr.textureBounds.Centered();
			}, ALC::BulletTypes<BulletDeleterComponent, NormalBullet>());
		}

	}

	// homing bullets
	if (GetShootButton())	m_homingShootTimer += ts * slowmult;
	else					m_homingShootTimer = 0.0f;
	if (m_homingShootTimer > m_homingShootSpeed) {
		m_homingShootTimer -= m_homingShootSpeed;
		delay = m_homingShootTimer;
		auto& selftr = self.GetComponent<ALC::Transform2D>();
		auto tex = m_spellsTexture;

		ShooterBehavior::SetDefaultPosition(selftr.position);
		ShooterBehavior::SetDefaultVelocity(ALC::vec2(0.0f, 800.0f) * slowmult);
		ShooterBehavior::Shoot(self, 1, [delay, tex](ALC::Entity e) {
			// update body collision
			auto& body = e.GetComponent<ALC::BulletBody>();
			body.radius = 18.0f;

			// update position
			auto& tr = e.GetComponent<ALC::Transform2D>();
			tr.position += body.velocity * delay;

			// update sprite
			auto& spr = e.GetComponent<ALC::SpriteComponent>();
			spr.color.a = 0.3f;
			spr.texture = tex;
			spr.textureBounds = ALC::rect(16.0f, 0.0f, 31.0f, 15.0f);
			spr.bounds = spr.textureBounds.Centered();

		}, ALC::BulletTypes<BulletDeleterComponent, HomingBullet>());
	}

}

#pragma endregion

#pragma region Spinning State

void AliceChara::StateBeginSpinning(const State laststate, ALC::Entity self, ALC::Timestep ts) {
	m_basicShootTimer = 0.0f;

	m_isRepositioning = false;
	for (size_t i = 0; i < m_pointShooters.size(); i++) {
		auto& point = m_pointShooters[i];

		// decide if it should reposition
		const float targetrot = (i == 0 ? 90.0f : 270.0f);
		if (!ALC::NearlyEqual(point.rotation, targetrot))
			m_isRepositioning = true;

		// reset the target
		if (i == 0) point.target = 0;
		else		point.target = 3;
	}
}
void AliceChara::StateStepSpinning(ALC::Entity self, ALC::Timestep ts) {
	// make sure the points are in the right position
	if (m_isRepositioning) {
		float speed = m_spinspeed * 3.0f;
		// tracks if its done repositioning
		bool confirm = true;

		// reposition
		for (size_t i = 0; i < m_pointShooters.size(); i++) {
			auto& point = m_pointShooters[i];
			const float targetrot = (i == 0 ? 90.0f : 270.0f);
			point.rotation = RotateTowards(point.rotation, targetrot, speed * ts);
			// done repose
			if (ALC::NearlyEqual(point.rotation, targetrot, speed * ts))
				point.rotation = targetrot;
			// not done repos
			else confirm = false;
		}

		if (confirm)	m_isRepositioning = false;
		else			return;
	}

	// dont do anything if not shooting
	if (!GetShootButton()) return;

	float slowmult = (GetSlowButton() ? 1.13f : 1.0f);
	auto& tr = self.GetComponent<ALC::Transform2D>();

	// basic shooting
	if (GetShootButton())	m_basicShootTimer += ts * slowmult;
	else					m_basicShootTimer = 0.0f;
	while (m_basicShootTimer > m_basicShootSpeed) {
		m_basicShootTimer -= m_basicShootSpeed;
		float delay = m_basicShootTimer;
		ShooterBehavior::SetDefaultVelocity(ALC::vec2(0.0f, 800.0f) * slowmult);
		auto tex = m_spellsTexture;
		ShooterBehavior::SetDefaultPosition(ALC::vec2(tr.position.x, tr.position.y + 16.5f));
		ShooterBehavior::Shoot(self, 1, [delay, tex](ALC::Entity e) {
			// update body collision
			auto& body = e.GetComponent<ALC::BulletBody>();
			body.radius = 18.0f;

			// update position
			auto& tr = e.GetComponent<ALC::Transform2D>();
			tr.position += body.velocity * delay;

			// update sprite
			auto& spr = e.GetComponent<ALC::SpriteComponent>();
			spr.color.a = 0.3f;
			spr.texture = tex;

			//spr.textureBounds = ALC::rect(0.0f, 16.0f, 15.0f, 31.0f);
			spr.textureBounds = ALC::rect(16.0f, 16.0f, 31.0f, 31.0f);
			spr.bounds = spr.textureBounds.Centered();
		}, ALC::BulletTypes<BulletDeleterComponent>());
	}

	// spin the point shooters
	for (size_t i = 0; i < m_pointShooters.size(); i++) {
		auto& point = m_pointShooters[i];

		// spin and wrap
		point.rotation += m_spinspeed * ts * slowmult;
		while (point.rotation > 360.0f) point.rotation -= 360.0f;

		float shootA = 0.0f, shootB = 0.0f, shootC = 0.0f;

		constexpr float angleA = 150.0f;
		constexpr float angleB = 180.0f;
		constexpr float angleC = 210.0f;

		// shoot when they reach certain positions
		if (point.target == 0 && point.rotation > angleA) {
			shootA = (point.rotation - angleA) / (m_spinspeed); // gives delay
			point.target = 1;
		}
		if (point.target == 1 && point.rotation > angleB) {
			shootB = (point.rotation - angleB) / (m_spinspeed); // gives delay
			point.target = 2;
		}
		if (point.target == 2 && point.rotation > angleC) {
			shootC = (point.rotation - angleC) / (m_spinspeed); // gives delay
			point.target = 3;
		}
		if (point.target == 3 && point.rotation < angleA) {
			point.target = 0; // reset
		}

		#pragma region Shooting the positional bullets

		ALC::vec2 vel = ALC::vec2(0.0f, 800.0f) * slowmult;
		auto tex = m_spellsTexture;
		ShooterBehavior::SetDefaultVelocity(vel);

		if (shootA > 0.0f) {
			auto pos = tr.position + ALC::vec2(glm::rotateZ(VEC3_DOWN * point.distance, ALC_TO_RADIANS(angleA))) + vel * shootA;
			pos.y = tr.position.y + point.distance;
			ShooterBehavior::SetDefaultPosition(pos);
			ShooterBehavior::Shoot(self, 1, [shootA, tex](ALC::Entity e) {
				// update body collision
				auto& body = e.GetComponent<ALC::BulletBody>();
				body.radius = 4.5f;
				body.damage = 2.0f;

				// update position
				auto& tr = e.GetComponent<ALC::Transform2D>();
				tr.position += body.velocity * shootA;

				// update sprite
				auto& spr = e.GetComponent<ALC::SpriteComponent>();
				spr.color.a = 0.3f;
				spr.texture = tex;
				spr.textureBounds = ALC::rect(0.0f, 0.0f, 15.0f, 15.0f);
				spr.bounds = spr.textureBounds.Centered();
			}, ALC::BulletTypes<BulletDeleterComponent>());
		}

		if (shootB > 0.0f) {
			auto pos = tr.position + ALC::vec2(glm::rotateZ(VEC3_DOWN * point.distance, ALC_TO_RADIANS(angleB))) + vel * shootB;
			pos.y = tr.position.y + point.distance;
			ShooterBehavior::SetDefaultPosition(pos);
			ShooterBehavior::Shoot(self, 1, [shootB, tex](ALC::Entity e) {
				// update body collision
				auto& body = e.GetComponent<ALC::BulletBody>();
				body.radius = 18.0f;
				body.damage = 2.0f;

				// update position
				auto& tr = e.GetComponent<ALC::Transform2D>();
				tr.position += body.velocity * shootB;

				// update sprite
				auto& spr = e.GetComponent<ALC::SpriteComponent>();
				spr.color.a = 0.3f;
				spr.texture = tex;
				spr.textureBounds = ALC::rect(0.0f, 0.0f, 15.0f, 15.0f);
				spr.bounds = spr.textureBounds.Centered();
			}, ALC::BulletTypes<BulletDeleterComponent>());
		}

		if (shootC > 0.0f) {
			auto pos = tr.position + ALC::vec2(glm::rotateZ(VEC3_DOWN * point.distance, ALC_TO_RADIANS(angleC))) + vel * shootC;
			pos.y = tr.position.y + point.distance;
			ShooterBehavior::SetDefaultPosition(pos);
			ShooterBehavior::Shoot(self, 1, [shootC, tex](ALC::Entity e) {
				// update body collision
				auto& body = e.GetComponent<ALC::BulletBody>();
				body.radius = 18.0f;
				body.damage = 2.0f;

				// update position
				auto& tr = e.GetComponent<ALC::Transform2D>();
				tr.position += body.velocity * shootC;

				// update sprite
				auto& spr = e.GetComponent<ALC::SpriteComponent>();
				spr.color.a = 0.3f;
				spr.texture = tex;
				spr.textureBounds = ALC::rect(0.0f, 0.0f, 15.0f, 15.0f);
				spr.bounds = spr.textureBounds.Centered();
			}, ALC::BulletTypes<BulletDeleterComponent>());
		}

		#pragma endregion

	}

}

#pragma endregion

#pragma region Shield State

void AliceChara::StateBeginShield(const State laststate, ALC::Entity self, ALC::Timestep ts) {
	// we have some invulnerability
	SetInvuln(7.0f);
	SetShouldFlashOnInvuln(false);
	// reuse this timer
	m_basicShootTimer = 0.0f;
	// visibility
	auto shield = GetRegistry().GetEntity(m_shieldEntity);
	auto& sprite = shield.GetComponent<ALC::SpriteComponent>();
	sprite.shouldDraw = true;
}
void AliceChara::StateEndShield(const State nextstate, ALC::Entity self, ALC::Timestep ts) {
	// visibility
	auto shield = GetRegistry().GetEntity(m_shieldEntity);
	auto& sprite = shield.GetComponent<ALC::SpriteComponent>();
	sprite.shouldDraw = false;
}
void AliceChara::StateStepShield(ALC::Entity self, ALC::Timestep ts) {
	auto shield = GetRegistry().GetEntity(m_shieldEntity);
	auto [transform, sprite] = shield.GetComponent<ALC::Transform2D, ALC::SpriteComponent>();

	// should shield state end?
	if (!IsInvuln()) m_activeSpell.ChangeState(m_lastSpell);

	// update shield size

	m_basicShootTimer += ts * 5.0f;

	ALC::rect bounds = sprite.textureBounds.Centered();

	// (sin(timer) + 1.0f) * 0.5f	// generates a nuber from 0 to 1
	// * 5.0f						// scales it from 0 to 5
	bounds.min.x = -(bounds.max.x += (sin(m_basicShootTimer) + 1.0f) * 0.5f * 5.0f);
	bounds.min.y = -(bounds.max.y += (cos(m_basicShootTimer) + 1.0f) * 0.5f * 5.0f);

	sprite.bounds = bounds;

	// update pos
	auto& selfTransform = self.GetComponent<ALC::Transform2D>();
	transform.position = selfTransform.position;

}

#pragma endregion

//#pragma region Death State
//
//void AliceChara::StateBeginDeath(const State laststate, ALC::Entity self, ALC::Timestep ts) {
//
//}
//void AliceChara::StateStepDeath(ALC::Entity self, ALC::Timestep ts) {
//
//}
//
//#pragma endregion

ALC::vec2 AliceChara::PointShooter::CalcPosition(const ALC::vec2& playerpos) {
	// set position
	return playerpos + ALC::vec2(glm::rotateZ(VEC3_DOWN * distance, ALC_TO_RADIANS(rotation)));
}
