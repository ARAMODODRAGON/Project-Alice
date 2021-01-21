#include "AliceChara.hpp"
#include <glm\gtx\rotate_vector.hpp>
#include "../Systems/BulletDeleterSystem.hpp"
#include "../Systems/Bullet Types/HomingBulletSystem.hpp"

#define VEC3_DOWN ALC::vec3(0.0f, -1.0f, 0.0f)

AliceChara::AliceChara()
	: m_basicShootTimer(0.0f)
	, m_homingShootTimer(0.0f)
	, m_basicShootSpeed(0.04f)
	, m_homingShootSpeed(0.12f)
	, m_rotationspeed(300.0f)
	, m_spinspeed(1200.0f)
	, m_isRepositioning(false)
	, m_activeSpell(this, Spell::Homing) {
	// bind states
	m_activeSpell.Bind(Spell::Homing, &AliceChara::StateStepHoming, &AliceChara::StateBeginHoming);
	m_activeSpell.Bind(Spell::Spinning, &AliceChara::StateStepSpinning, &AliceChara::StateBeginSpinning);
}

AliceChara::~AliceChara() { }

void AliceChara::Start(ALC::Entity self) {
	// call base
	Character::Start(self);
	using CM = ALC::ContentManager;

	// get textures
	m_bulletTexture = CM::LoadTexture("Resources/Textures/Alice-Spells.png");

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
		point.distance = 57.0f;

		// set position
		tr.position = point.CalcPosition(selftr.position);

		// set the sprite stuff
		spr.bounds = ALC::rect(10.0f);
		spr.texture = m_bulletTexture;

		if (i == 0) {
			spr.textureBounds = ALC::rect(ALC::vec2(16.0f, 32.0f), ALC::vec2(32.0f, 48.0f));
		} else {
			spr.textureBounds = ALC::rect(ALC::vec2(0.0f, 48.0f), ALC::vec2(15.0f, 63.0f));
		}

	}
}

void AliceChara::Update(ALC::Entity self, ALC::Timestep ts) {
	UpdateMovement(self, ts);

	// toggle state
	if (GetModButton().Pressed()) {
		switch (m_activeSpell.GetState()) {
			case Spell::Homing:
				m_activeSpell.ChangeState(Spell::Spinning);
				break;
			case Spell::Spinning:
				m_activeSpell.ChangeState(Spell::Homing);
				break;
			default: ALC_DEBUG_WARNING("failed to switch states"); break;
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

void AliceChara::LateUpdate(ALC::Entity self, ALC::Timestep ts) {
	UpdateCollisions(self, ts);
	UpdateSprites(self, ts);
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

void AliceChara::StateBeginHoming(const Spell laststate, ALC::Entity self, ALC::Timestep ts) {
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
		ShooterBehavior::SetBulletTypes<BulletDeleterComponent>();
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
			auto tex = m_bulletTexture;
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
				spr.bounds = ALC::rect(14.0f);
				spr.color.a = 0.3f;
				spr.texture = tex;

				if (i == 0) spr.textureBounds = ALC::rect(0.0f, 16.0f, 15.0f, 31.0f);
				else		spr.textureBounds = ALC::rect(16.0f, 16.0f, 31.0f, 31.0f);
			});
		}

	}

	// homing bullets
	if (GetShootButton())	m_homingShootTimer += ts * slowmult;
	else					m_homingShootTimer = 0.0f;
	if (m_homingShootTimer > m_homingShootSpeed) {
		m_homingShootTimer -= m_homingShootSpeed;
		delay = m_homingShootTimer;
		auto& selftr = self.GetComponent<ALC::Transform2D>();
		auto tex = m_bulletTexture;

		ShooterBehavior::SetDefaultPosition(selftr.position);
		ShooterBehavior::SetBulletTypes<BulletDeleterComponent, HomingBullet>();
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
			spr.bounds = ALC::rect(14.0f);
			spr.color.a = 0.3f;
			spr.texture = tex;
			spr.textureBounds = ALC::rect(16.0f, 0.0f, 31.0f, 15.0f);
		});
	}

}

void AliceChara::StateBeginSpinning(const Spell laststate, ALC::Entity self, ALC::Timestep ts) {
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
		ShooterBehavior::SetBulletTypes<BulletDeleterComponent>();
		ShooterBehavior::SetDefaultVelocity(ALC::vec2(0.0f, 800.0f) * slowmult);
		auto tex = m_bulletTexture;
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
			spr.bounds = ALC::rect(14.0f);
			spr.color.a = 0.3f;
			spr.texture = tex;

			//spr.textureBounds = ALC::rect(0.0f, 16.0f, 15.0f, 31.0f);
			spr.textureBounds = ALC::rect(16.0f, 16.0f, 31.0f, 31.0f);
		});
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

		ShooterBehavior::SetBulletTypes<BulletDeleterComponent>();
		ALC::vec2 vel = ALC::vec2(0.0f, 800.0f) * slowmult;
		auto tex = m_bulletTexture;
		ShooterBehavior::SetDefaultVelocity(vel);

		if (shootA > 0.0f) {
			auto pos = tr.position + ALC::vec2(glm::rotateZ(VEC3_DOWN * point.distance, ALC_TO_RADIANS(angleA))) + vel * shootA;
			pos.y = tr.position.y + point.distance;
			ShooterBehavior::SetDefaultPosition(pos);
			ShooterBehavior::Shoot(self, 1, [shootA, tex](ALC::Entity e) {
				// update body collision
				auto& body = e.GetComponent<ALC::BulletBody>();
				body.radius = 4.5f;

				// update position
				auto& tr = e.GetComponent<ALC::Transform2D>();
				tr.position += body.velocity * shootA;

				// update sprite
				auto& spr = e.GetComponent<ALC::SpriteComponent>();
				spr.bounds = ALC::rect(14.0f);
				spr.color.a = 0.3f;
				spr.texture = tex;
				spr.textureBounds = ALC::rect(0.0f, 0.0f, 15.0f, 15.0f);
			});
		}

		if (shootB > 0.0f) {
			auto pos = tr.position + ALC::vec2(glm::rotateZ(VEC3_DOWN * point.distance, ALC_TO_RADIANS(angleB))) + vel * shootB;
			pos.y = tr.position.y + point.distance;
			ShooterBehavior::SetDefaultPosition(pos);
			ShooterBehavior::Shoot(self, 1, [shootB, tex](ALC::Entity e) {
				// update body collision
				auto& body = e.GetComponent<ALC::BulletBody>();
				body.radius = 18.0f;

				// update position
				auto& tr = e.GetComponent<ALC::Transform2D>();
				tr.position += body.velocity * shootB;

				// update sprite
				auto& spr = e.GetComponent<ALC::SpriteComponent>();
				spr.bounds = ALC::rect(14.0f);
				spr.color.a = 0.3f;
				spr.texture = tex;
				spr.textureBounds = ALC::rect(0.0f, 0.0f, 15.0f, 15.0f);
			});
		}

		if (shootC > 0.0f) {
			auto pos = tr.position + ALC::vec2(glm::rotateZ(VEC3_DOWN * point.distance, ALC_TO_RADIANS(angleC))) + vel * shootC;
			pos.y = tr.position.y + point.distance;
			ShooterBehavior::SetDefaultPosition(pos);
			ShooterBehavior::Shoot(self, 1, [shootC, tex](ALC::Entity e) {
				// update body collision
				auto& body = e.GetComponent<ALC::BulletBody>();
				body.radius = 18.0f;

				// update position
				auto& tr = e.GetComponent<ALC::Transform2D>();
				tr.position += body.velocity * shootC;

				// update sprite
				auto& spr = e.GetComponent<ALC::SpriteComponent>();
				spr.bounds = ALC::rect(14.0f);
				spr.color.a = 0.3f;
				spr.texture = tex;
				spr.textureBounds = ALC::rect(0.0f, 0.0f, 15.0f, 15.0f);
			});
		}

		#pragma endregion

	}

}

ALC::vec2 AliceChara::PointShooter::CalcPosition(const ALC::vec2& playerpos) {
	// set position
	return playerpos + ALC::vec2(glm::rotateZ(VEC3_DOWN * distance, ALC_TO_RADIANS(rotation)));
}
