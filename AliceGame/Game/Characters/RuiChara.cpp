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
	: m_spell(this, Spell::Homing)
	, m_homingfireIndex(0)
	, m_homingShootTimer(0.0f)
	, m_homingShootSpeed(0.018f) {
	// bind states
	m_spell.Bind(Spell::Homing, &RuiChara::StateStepHoming, &RuiChara::StateBeginHoming);
}

RuiChara::~RuiChara() { }

void RuiChara::Start(ALC::Entity self) {
	Character::Start(self);
}

void RuiChara::Update(ALC::Entity self, ALC::Timestep ts) {
	UpdateMovement(self, ts);

	// update spell
	m_spell(self, ts);

}

void RuiChara::LateUpdate(ALC::Entity self, ALC::Timestep ts) {
	Character::UpdateSprites(self, ts);
}

void RuiChara::StateBeginHoming(const Spell last, ALC::Entity self, ALC::Timestep ts) {
	m_homingShootTimer = 0.0f;
	m_homingfireIndex = 0;
}
void RuiChara::StateStepHoming(ALC::Entity self, ALC::Timestep ts) {
	auto& transform = self.GetComponent<ALC::Transform2D>();

	if (GetShootButton()) m_homingShootTimer += ts;
	else {
		m_homingShootTimer = 0.0f;
		m_homingfireIndex = 0;
	}

	while (m_homingShootTimer > m_homingShootSpeed) {
		m_homingShootTimer -= m_homingShootSpeed;

		SetBulletTypes<BulletDeleterComponent, HomingBullet>();
		const float angle = shoot_angle[shoot_pattern[m_homingfireIndex++ % shoot_pattern_size]];
		const ALC::vec2 vel = glm::rotate(ALC::vec2(0.0f, 660.0f), glm::radians(angle));

		SetDefaultPosition(transform.position + vel * m_homingShootTimer);
		SetDefaultVelocity(vel);

		// gets the next angle to shoot at

		Shoot(self, 1, [](ALC::Entity bullet) {
			auto& sprite = bullet.GetComponent<ALC::SpriteComponent>();
			sprite.bounds = ALC::rect(10.0f);
			auto& del = bullet.GetComponent<BulletDeleterComponent>();
			del.lifetime = 2.0f;
		});

	}

}
