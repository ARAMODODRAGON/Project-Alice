#ifndef ATK_WIND_SLASH_HPP
#define ATK_WIND_SLASH_HPP

#include "BTAttackSpell.hpp"

class BTWindSlash : public BTAttackSpell {
public:
	BTWindSlash();
	~BTWindSlash();

	void OnCreate(int _count) override;
	void Update(Button _shootButton) override;

};

#endif