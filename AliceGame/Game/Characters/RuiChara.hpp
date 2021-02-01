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

private:

	enum class Spell : ALC::uint8 {
		Homing
	};

	void StateBeginHoming(const Spell last, ALC::Entity self, ALC::Timestep ts);
	void StateStepHoming(ALC::Entity self, ALC::Timestep ts);

	ALC::EntityStateMachine<RuiChara, Spell> m_spell;
	ALC::uint8 m_homingfireIndex;
	float m_homingShootTimer;
	float m_homingShootSpeed;

};

#endif // !ALICE_CHARACTERS_RUICHARA_HPP