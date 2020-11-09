#ifndef BATTLE_ATTACK_SPELL_HPP
#define BATTLE_ATTACK_SPELL_HPP

#include "../BTSpell.hpp"
#include "../../ShootComponent.hpp"
#include "../../../Input/Button.hpp"

class BTAttackSpell : public BTSpell {
	type attackType;
	int allocateCount;

	ShootComponent* shootComponent;
public:
	BTAttackSpell();
	~BTAttackSpell();

	virtual void OnCreate(type _type, int _count) = 0;
	virtual void Update(Button _shootButton) = 0;

	ShootComponent* GetShootComponent();
	void SetShootComponent(ShootComponent* _component);

	RTTR_ENABLE(BTSpell) RTTR_REGISTRATION_FRIEND
};

#endif