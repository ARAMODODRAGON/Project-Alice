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
	, m_spinningSpeed(0.5f)
	, m_spinningSpeedMultiplier(0.9f)
	, m_activeSpell(this, Spell::Homing) {
	// bind states
	m_activeSpell.Bind(Spell::Homing, &AliceChara::StateStepHoming);
	m_activeSpell.Bind(Spell::Spinning, &AliceChara::StateStepSpinning);
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
		point.distance = 17.0f;

		// set position
		tr.position = point.CalcPosition(selftr.position);

		// set the sprite stuff
		spr.bounds = ALC::rect(4.0f);
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

static float RotateTowards(float curangle, const float target, const float speed) {
	if (ALC::NearlyEqual(curangle, target, speed))
		curangle = target;
	else if (curangle > target)
		curangle -= speed;
	else if (curangle < target)
		curangle += speed;
	return curangle;
}

void AliceChara::StateStepHoming(ALC::Entity self, ALC::Timestep ts) {
	float slowmult = 1.13f;

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
		ShooterBehavior::SetDefaultVelocity(ALC::vec2(0.0f, 240.0f) * slowmult);
	}

	// get self position
	auto& reg = GetRegistry();

	for (size_t i = 0; i < m_pointShooters.size(); i++) {
		// relevant info
		auto& point = m_pointShooters[i];
		auto entity = reg.GetEntity(point.entityID);
		auto& tr = entity.GetComponent<ALC::Transform2D>();

		constexpr float rotationspeed = 300.0f;

		// confirm they are at the right position;
		if (!GetSlowButton()) {
			if (i == 0)
				point.rotation = RotateTowards(point.rotation, 90.0f, rotationspeed * ts);
			else
				point.rotation = RotateTowards(point.rotation, 270.0f, rotationspeed * ts);
		} else {
			if (i == 0)
				point.rotation = RotateTowards(point.rotation, 150.0f, rotationspeed * ts);
			else
				point.rotation = RotateTowards(point.rotation, 210.0f, rotationspeed * ts);
		}

		// shoot
		if (basicShoot) {
			auto tex = m_bulletTexture;
			ShooterBehavior::SetDefaultPosition(tr.position);
			ShooterBehavior::Shoot(self, 1, [i, delay, tex](ALC::Entity e) {
				// update body collision
				auto& body = e.GetComponent<ALC::BulletBody>();
				body.mask = ALC::Layermask32::NONE;
				body.radius = 4.5f;

				// update position
				auto& tr = e.GetComponent<ALC::Transform2D>();
				tr.position += body.velocity * delay;

				// update sprite
				auto& spr = e.GetComponent<ALC::SpriteComponent>();
				spr.bounds = ALC::rect(4.0f);
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
		ShooterBehavior::SetDefaultVelocity(ALC::vec2(0.0f, 240.0f) * slowmult);
		ShooterBehavior::Shoot(self, 1, [delay, tex](ALC::Entity e) {
			// update body collision
			auto& body = e.GetComponent<ALC::BulletBody>();
			body.mask = ALC::Layermask32::NONE;
			body.radius = 4.5f;

			// update position
			auto& tr = e.GetComponent<ALC::Transform2D>();
			tr.position += body.velocity * delay;

			// update sprite
			auto& spr = e.GetComponent<ALC::SpriteComponent>();
			spr.bounds = ALC::rect(4.0f);
			spr.color.a = 0.3f;
			spr.texture = tex;
			spr.textureBounds = ALC::rect(16.0f, 0.0f, 31.0f, 15.0f);
		});
	}

}

void AliceChara::StateStepSpinning(ALC::Entity self, ALC::Timestep ts) { }

ALC::vec2 AliceChara::PointShooter::CalcPosition(const ALC::vec2& playerpos) {
	// set position
	return playerpos + ALC::vec2(glm::rotateZ(VEC3_DOWN * distance, ALC_TO_RADIANS(rotation)));
}
