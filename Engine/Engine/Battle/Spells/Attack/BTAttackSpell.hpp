#ifndef BATTLE_ATTACK_SPELL_HPP
#define BATTLE_ATTACK_SPELL_HPP

#include "../BTSpell.hpp"
#include "../../ShootComponent.hpp"

class BTAttackSpell : public BTSpell {
	rttr::type attackType;
	int allocateCount;

	ShootComponent* shootComponent;
public:
	BTAttackSpell(rttr::type _type, int _count);
	~BTAttackSpell() override;

	void Update();

	ShootComponent* GetShootComponent();
	void SetShootComponent(ShootComponent* _component);

	RTTR_ENABLE(BTSpell) RTTR_REGISTRATION_FRIEND
};

#endif