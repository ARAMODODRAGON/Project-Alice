#ifndef ATK_BOOMERANG_HPP
#define ATK_BOOMERANG_HPP

#include "BTAttackSpell.hpp"

class Boomerang : public BTAttackSpell {
public:
	void Update(Button _shootButton) override;
};

#endif