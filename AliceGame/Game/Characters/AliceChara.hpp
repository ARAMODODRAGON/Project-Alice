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

private:

	float m_basicShootTimer;
	float m_homingShootTimer;
	float m_basicShootSpeed;
	float m_homingShootSpeed;
	float m_rotationspeed;
	float m_spinspeed;
	bool m_isRepositioning;

	enum class Spell : ALC::uint8 {
		Homing,
		Spinning
	};
	void StateBeginHoming(const Spell laststate, ALC::Entity self, ALC::Timestep ts);
	void StateStepHoming(ALC::Entity self, ALC::Timestep ts);
	void StateBeginSpinning(const Spell laststate, ALC::Entity self, ALC::Timestep ts);
	void StateStepSpinning(ALC::Entity self, ALC::Timestep ts);

	ALC::EntityStateMachine<AliceChara, Spell> m_activeSpell;

	struct PointShooter {
		ALC::EntityID entityID;
		float rotation; // rotation around player
		float distance; // distance from player
		ALC::uint32 target; // when spinning to track the target
		ALC::vec2 CalcPosition(const ALC::vec2& playerpos);
	};

	ALC::array<PointShooter, 2> m_pointShooters;

	ALC::Texture m_bulletTexture;

	static float RotateTowards(float curangle, const float target, const float speed);
};

#endif // !ALICE_CHARACTERS_ALICECHARA_HPP