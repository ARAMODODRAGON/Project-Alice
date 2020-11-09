#ifndef ATK_WIND_SLASH_HPP
#define ATK_WIND_SLASH_HPP

#include "BTAttackSpell.hpp"

class WindSlash : public BTAttackSpell {
public:
	void Update(Button _shootButton) override;
};

#endif