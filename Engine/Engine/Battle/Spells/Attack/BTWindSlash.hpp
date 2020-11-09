#ifndef ATK_WIND_SLASH_HPP
#define ATK_WIND_SLASH_HPP

#include "BTAttackSpell.hpp"

class BTWindSlash : public BTAttackSpell {
public:
	void Update(Button _shootButton) override;

	RTTR_ENABLE(BTAttackSpell) RTTR_REGISTRATION_FRIEND
};

#endif