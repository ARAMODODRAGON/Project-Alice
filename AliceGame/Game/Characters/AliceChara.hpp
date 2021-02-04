#ifndef ALICE_CHARACTERS_ALICECHARA_HPP
#define ALICE_CHARACTERS_ALICECHARA_HPP
#include "Character.hpp"

class AliceChara final : public Character {
public:

	AliceChara();
	~AliceChara();

	virtual void Start(ALC::Entity self);
	virtual void Update(ALC::Entity self, ALC::Timestep ts);
	virtual void LateUpdate(ALC::Entity self, ALC::Timestep ts);

	ALC::rect GetAttackTargetRect() const override;
	ALC::rect GetDefenceTargetRect() const override;
	ALC::rect GetDefenceTargetRectCooldown() const override;
	ALC::rect GetHeartTargetRect() const override;

private:

	enum class Spell : ALC::uint8 {
		Homing, Spinning, Shield
	};

	void StateBeginHoming(const Spell laststate, ALC::Entity self, ALC::Timestep ts);
	void StateStepHoming(ALC::Entity self, ALC::Timestep ts);
	void StateBeginSpinning(const Spell laststate, ALC::Entity self, ALC::Timestep ts);
	void StateStepSpinning(ALC::Entity self, ALC::Timestep ts);
	void StateBeginShield(const Spell laststate, ALC::Entity self, ALC::Timestep ts);
	void StateEndShield(const Spell nextstate, ALC::Entity self, ALC::Timestep ts);
	void StateStepShield(ALC::Entity self, ALC::Timestep ts);

	struct PointShooter {
		ALC::EntityID entityID;
		float rotation; // rotation around player
		float distance; // distance from player
		ALC::uint32 target; // when spinning to track the target
		ALC::vec2 CalcPosition(const ALC::vec2& playerpos);
	};

	float m_basicShootTimer;
	float m_homingShootTimer;
	float m_basicShootSpeed;
	float m_homingShootSpeed;
	float m_rotationspeed;
	float m_spinspeed;
	bool m_isRepositioning;
	ALC::EntityStateMachine<AliceChara, Spell> m_activeSpell;
	ALC::array<PointShooter, 2> m_pointShooters;
	ALC::Texture m_bulletTexture;
	Spell m_lastSpell;
	float m_shieldCharge;
	float m_shieldChargeRate;
	ALC::EntityID m_shieldEntity;

	static float RotateTowards(float curangle, const float target, const float speed);
};

#endif // !ALICE_CHARACTERS_ALICECHARA_HPP