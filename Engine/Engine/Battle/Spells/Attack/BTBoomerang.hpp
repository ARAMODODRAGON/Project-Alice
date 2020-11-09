#ifndef ATK_BOOMERANG_HPP
#define ATK_BOOMERANG_HPP

#include "BTAttackSpell.hpp"

class BTBoomerang : public BTAttackSpell {
public:
	void Update(Button _shootButton) override;

	RTTR_ENABLE(BTAttackSpell) RTTR_REGISTRATION_FRIEND
};

#endif