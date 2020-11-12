#ifndef BATTLE_DEFENCE_SPELL_HPP
#define BATTLE_DEFENCE_SPELL_HPP

#include "../BTSpell.hpp"

class BTDefenceSpell : public BTSpell {

public:
	BTDefenceSpell();
	~BTDefenceSpell();

	virtual void Use() = 0;

	RTTR_ENABLE(BTSpell) RTTR_REGISTRATION_FRIEND
};

#endif