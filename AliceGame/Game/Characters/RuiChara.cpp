#include "RuiChara.hpp"
#include "../Systems/BulletDeleterSystem.hpp"
#include "../Systems/Bullet Types/HomingBulletSystem.hpp"

// decides the firing pattern for rui's homing attack
static constexpr ALC::uint32 shoot_pattern[] = {
	0,4,1,3,2,
	1,4,0,2,3,
	4,3,2,1,0,
	3,1,2,4,0,
	2,3,0,4,1
};
static constexpr ALC::uint32 shoot_pattern_size = sizeof(shoot_pattern) / sizeof(ALC::uint32);

// the firing angles for her homing attack
static constexpr float shoot_angle[] = {
	-60.0f,-30.0f, 0.0f, 30.0f, 60.0f
};

RuiChara::RuiChara()
	: m_spell(this, Spell::Basic)
	, m_fireIndex(0)
	, m_homingShootTimer(0.0f)
	, m_homingShootSpeed(0.01f)
	, m_slowHomingShootSpeed(0.03f)
	, m_shootTimer(0.0f)
	, m_shootSpeed(0.03f)
	, m_spelltex(nullptr)
	, m_rapidTimer(0.0f)
	, m_rapidLength(4.0f)
	, m_rapidDelay(10.0f)
	, m_rapidMoveMultiplier(0.1f)
	, m_flowerEntity(nullptr)
	, m_flowerDefaultOffset(50.0f)
	, m_flowerTargetPos(0.0f, -m_flowerDefaultOffset)
	, m_flowerPop(false)
	, m_flowerMoveSpeed(380.0f) {
	// bind states
	m_spell.Bind(Spell::Basic, &RuiChara::StateStepBasic, &RuiChara::StateBeginBasic);
	m_spell.Bind(Spell::Rapid, &RuiChara::StateStepRapid, &RuiChara::StateBeginRapid);
	SetSlowScalar(0.3f);
}

RuiChara::~RuiChara() { }

void RuiChara::Start(ALC::Entity self) {
	Character::Start(self);

	// load spell texture
	m_spelltex = ALC::ContentManager::LoadTexture("Resources/Textures/Spells.png");

	// create flower object with components
	auto flower = GetRegistry().Create();
	m_flowerEntity = flower.GetComponent<ALC::EntityInfo>().GetID();
	auto& ftr = flower.AddComponent<ALC::Transform2D>();
	auto& fspr = flower.AddComponent<ALC::SpriteComponent>();

	auto& transform = self.GetComponent<ALC::Transform2D>();
	ftr.position = transform.position + m_flowerTargetPos;

	fspr.texture = m_spelltex;
	fspr.textureBounds = ALC::rect(64.0f, 80.0f, 79.0f, 95.0f);
	fspr.bounds = ALC::rect(10.0f);

}

void RuiChara::Update(ALC::Entity self, ALC::Timestep ts) {
	if (m_spell.GetState() != Spell::Rapid)
		UpdateMovement(self, ts);
	else
		UpdateMovement(self, ts, GetInputAxis(), GetMaxSpeed() * m_rapidMoveMultiplier);

	// update spell
	m_spell(self, ts);

	// move flower to position
	auto flower = GetFlower();
	auto& ftr = flower.GetComponent<ALC::Transform2D>();
	auto& fspr = flower.GetComponent<ALC::SpriteComponent>();
	auto& transform = self.GetComponent<ALC::Transform2D>();

	// move to
	if (m_shouldReposition) {
		ALC::vec2 dir = (m_flowerTargetPos + transform.position) - ftr.position;
		if (!ALC::NearlyZero(dir)) dir = glm::normalize(dir) * m_flowerMoveSpeed;
		ftr.position += dir * ts;

		if (ALC::NearlyEqual(ftr.position, m_flowerTargetPos + transform.position, m_flowerMoveSpeed * ts)) {
			ftr.position = m_flowerTargetPos + transform.position;
			m_shouldReposition = false;
		}
	}
	// set position
	else {
		ftr.position = m_flowerTargetPos + transform.position;
	}

	// update flower sprite
	if (m_flowerPop)
		fspr.textureBounds = ALC::rect(80.0f, 80.0f, 95.0f, 95.0f);
	else
		fspr.textureBounds = ALC::rect(64.0f, 80.0f, 79.0f, 95.0f);

}

void RuiChara::LateUpdate(ALC::Entity self, ALC::Timestep ts) {
	Character::UpdateSprites(self, ts);
}

ALC::rect RuiChara::GetAttackTargetRect() const {
	ALC_DEBUG_WARNING("function not implemented");
	return ALC::rect(4.0f, 4.0f, 27.0f, 27.0f);
}
ALC::rect RuiChara::GetDefenceTargetRect() const {
	ALC_DEBUG_WARNING("function not implemented");
	return ALC::rect(4.0f, 4.0f, 27.0f, 27.0f);
}
ALC::rect RuiChara::GetHeartTargetRect() const {
	if (IsInvuln()) return ALC::rect(117.0f, 7.0f, 125.0f, 14.0f);
	else			return ALC::rect(117.0f, 18.0f, 125.0f, 25.0f);
}
ALC::Entity RuiChara::GetFlower() {
	return GetRegistry().GetEntity(m_flowerEntity);
}

void RuiChara::StateBeginBasic(const Spell last, ALC::Entity self, ALC::Timestep ts) {
	m_homingShootTimer = 0.0f;
	m_shootTimer = 0.0f;
	m_fireIndex = 0;
	m_flowerTargetPos = ALC::vec2(0.0f, -m_flowerDefaultOffset);
	m_shouldReposition = true;
}
void RuiChara::StateStepBasic(ALC::Entity self, ALC::Timestep ts) {
	if (m_shouldReposition) return;

	auto& transform = self.GetComponent<ALC::Transform2D>();
	ALC::vec2 flowerpos = (m_flowerTargetPos + transform.position);

	m_rapidTimer -= ts;
	if (m_rapidTimer < 0.0f && GetModButton().Pressed()) {
		m_spell.ChangeState(Spell::Rapid);
	}

	// shoot normal bullets
	if (GetShootButton()) m_shootTimer += ts;
	else {
		m_shootTimer = 0.0f;
	}
	while (m_shootTimer > m_shootSpeed) {
		m_shootTimer -= m_shootSpeed;

		ALC::vec2 vel(0.0f, 600.0f);
		auto tex = m_spelltex;

		SetDefaultPosition(transform.position + vel * m_shootTimer);
		SetDefaultVelocity(vel);
		SetBulletTypes<BulletDeleterComponent>();

		Shoot(self, 1, [tex](ALC::Entity bullet) {
			auto& sprite = bullet.GetComponent<ALC::SpriteComponent>();
			sprite.bounds = ALC::rect(10.0f);
			sprite.texture = tex;
			sprite.textureBounds = ALC::rect(32.0f, 96.0f, 47.0f, 111.0f);
			sprite.color.a = 0.3f;
		});

	}

	// shoot homing bullets
	if (GetShootButton()) m_homingShootTimer += ts;
	else {
		m_flowerPop = false;
		m_homingShootTimer = 0.0f;
	}
	while (m_homingShootTimer > m_slowHomingShootSpeed) {
		m_homingShootTimer -= m_slowHomingShootSpeed;
		m_flowerPop = !m_flowerPop;

		// shoots left then right
		ALC::vec2 vel(400.0f * (m_fireIndex % 2 ? -1.0f : 1.0f), shoot_angle[shoot_pattern[m_fireIndex % shoot_pattern_size]]);
		m_fireIndex++;
		auto tex = m_spelltex;

		SetDefaultPosition(flowerpos + vel * m_homingShootTimer);
		SetDefaultVelocity(vel);
		SetBulletTypes<BulletDeleterComponent, HomingBullet>();

		Shoot(self, 1, [tex](ALC::Entity bullet) {
			auto& sprite = bullet.GetComponent<ALC::SpriteComponent>();
			sprite.bounds = ALC::rect(10.0f);
			sprite.texture = tex;
			sprite.textureBounds = ALC::rect(32.0f, 80.0f, 47.0f, 95.0f);
			sprite.color.a = 0.3f;
		});

	}

}

void RuiChara::StateBeginRapid(const Spell last, ALC::Entity self, ALC::Timestep ts) {
	m_homingShootTimer = 0.0f;
	m_fireIndex = 0;
	m_rapidTimer = m_rapidLength;
	m_flowerTargetPos = ALC::vec2(0.0f, m_flowerDefaultOffset);
	m_shouldReposition = true;
}
void RuiChara::StateStepRapid(ALC::Entity self, ALC::Timestep ts) {
	if (m_shouldReposition) return;

	auto& transform = self.GetComponent<ALC::Transform2D>();
	auto pos = m_flowerTargetPos + transform.position;

	m_rapidTimer -= ts;
	if (m_rapidTimer < 0.0f) {
		m_spell.ChangeState(Spell::Basic);
		m_rapidTimer = m_rapidDelay;
		return;
	}

	// shoot as much as required
	m_homingShootTimer += ts;
	while (m_homingShootTimer > m_homingShootSpeed) {
		m_homingShootTimer -= m_homingShootSpeed;
		m_flowerPop = !m_flowerPop;

		SetBulletTypes<BulletDeleterComponent, HomingBullet>();

		// gets the next angle to shoot at
		const float angle = shoot_angle[shoot_pattern[m_fireIndex++ % shoot_pattern_size]];
		const ALC::vec2 vel = glm::rotate(ALC::vec2(0.0f, 760.0f), glm::radians(angle));
		auto tex = m_spelltex;

		SetDefaultPosition(pos + vel * m_homingShootTimer);
		SetDefaultVelocity(vel);

		// shoot bitch
		Shoot(self, 1, [tex](ALC::Entity bullet) {
			auto& sprite = bullet.GetComponent<ALC::SpriteComponent>();
			sprite.bounds = ALC::rect(10.0f);
			auto& homing = bullet.GetComponent<HomingBullet>();
			homing.rotationSpeed = 270.0f;
			auto& del = bullet.GetComponent<BulletDeleterComponent>();
			del.lifetime = 2.0f;
			sprite.texture = tex;
			sprite.textureBounds = ALC::rect(32.0f, 80.0f, 47.0f, 95.0f);
			sprite.color.a = 0.3f;
		});

	}

}
