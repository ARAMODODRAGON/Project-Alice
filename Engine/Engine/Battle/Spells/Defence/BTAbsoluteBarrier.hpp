#ifndef DEF_ABSOLUTE_BARRIER_HPP
#define DEF_ABSOLUTE_BARRIER_HPP

#include "BTDefenceSpell.hpp"

class BTAbsoluteBarrier : public BTDefenceSpell {
public:
	BTAbsoluteBarrier();
	~BTAbsoluteBarrier();

	void Use() override;

};

#endif