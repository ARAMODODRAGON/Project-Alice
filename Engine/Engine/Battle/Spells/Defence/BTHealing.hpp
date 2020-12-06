#ifndef DEF_HEALING_HPP
#define DEF_HEALING_HPP

#include "BTDefenceSpell.hpp"

class BTHealing : public BTDefenceSpell {
public:
	BTHealing();
	~BTHealing();

	void Use() override;

};

#endif