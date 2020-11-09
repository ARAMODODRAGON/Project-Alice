#ifndef DEF_ABSOLUTE_BARRIER_HPP
#define DEF_ABSOLUTE_BARRIER_HPP

#include "BTDefenceSpell.hpp"

class BTAbsoluteBarrier : public BTDefenceSpell {
public:
	void Use() override;

	RTTR_ENABLE(BTDefenceSpell) RTTR_REGISTRATION_FRIEND
};

#endif