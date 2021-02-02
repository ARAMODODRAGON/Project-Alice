#ifndef ALICE_CHARACTERS_RUICHARA_HPP
#define ALICE_CHARACTERS_RUICHARA_HPP
#include "Character.hpp"

class RuiChara final : public Character {
public:

	RuiChara();
	~RuiChara();

	virtual void Start(ALC::Entity self);
	virtual void Update(ALC::Entity self, ALC::Timestep ts);
	virtual void LateUpdate(ALC::Entity self, ALC::Timestep ts);

	ALC::rect GetAttackTargetRect() const override;
	ALC::rect GetDefenceTargetRect() const override;
	ALC::rect GetHeartTargetRect() const override;

	ALC::Entity GetFlower();

private:

	enum class Spell : ALC::uint8 {
		Basic, Rapid
	};

	void StateBeginBasic(const Spell last, ALC::Entity self, ALC::Timestep ts);
	void StateStepBasic(ALC::Entity self, ALC::Timestep ts);
	void StateBeginRapid(const Spell last, ALC::Entity self, ALC::Timestep ts);
	void StateStepRapid(ALC::Entity self, ALC::Timestep ts);

	ALC::EntityStateMachine<RuiChara, Spell> m_spell;
	ALC::uint8 m_fireIndex;
	float m_homingShootTimer;
	float m_homingShootSpeed;
	float m_slowHomingShootSpeed;
	float m_shootTimer;
	float m_shootSpeed;
	ALC::Texture m_spelltex;

	// rapid
	float m_rapidTimer;
	float m_rapidLength;
	float m_rapidDelay;
	float m_rapidMoveMultiplier;

	// flower
	ALC::EntityID m_flowerEntity;
	float m_flowerDefaultOffset;
	ALC::vec2 m_flowerTargetPos;
	bool m_flowerPop;
	float m_flowerMoveSpeed;
	bool m_shouldReposition;

};

#endif // !ALICE_CHARACTERS_RUICHARA_HPP