#ifndef ATK_BOOMERANG_HPP
#define ATK_BOOMERANG_HPP

#include "BTAttackSpell.hpp"

class BTBoomerang : public BTAttackSpell {
public:
	BTBoomerang();
	~BTBoomerang();

	void OnCreate(int _count) override;
	void Update(Button _shootButton) override;

};

#endif