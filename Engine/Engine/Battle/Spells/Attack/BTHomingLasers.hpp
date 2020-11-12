#ifndef ATK_HOMING_LASERS_HPP
#define ATK_HOMING_LASERS_HPP

#include "BTAttackSpell.hpp"

class BTHomingLasers : public BTAttackSpell {
public:
	BTHomingLasers();
	~BTHomingLasers();

	void OnCreate(type _type, int _count) override;
	void Update(Button _shootButton) override;

	RTTR_ENABLE(BTAttackSpell) RTTR_REGISTRATION_FRIEND
};

#endif