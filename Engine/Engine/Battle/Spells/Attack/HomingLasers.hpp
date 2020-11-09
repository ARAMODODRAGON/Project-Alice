#ifndef ATK_HOMING_LASERS_HPP
#define ATK_HOMING_LASERS_HPP

#include "BTAttackSpell.hpp"

class HomingLasers : public BTAttackSpell {
public:
	void Update(Button _shootButton) override;
};

#endif