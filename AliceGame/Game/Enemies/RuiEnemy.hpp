#ifndef ALICE_ENEMIES_RUIENEMY_HPP
#define ALICE_ENEMIES_RUIENEMY_HPP
#include "Enemy.hpp"

class RuiEnemy final : public Enemy {
public:

	RuiEnemy();
	~RuiEnemy();

	virtual void Start(ALC::Entity self);
	virtual void Update(ALC::Entity self, ALC::Timestep ts);

};

#endif // !ALICE_ENEMIES_RUIENEMY_HPP