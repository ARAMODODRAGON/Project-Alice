#ifndef BATTLE_DEMO_ENEMY_CW_HPP
#define BATTLE_DEMO_ENEMY_CW_HPP
#include <Engine\Battle\BTEnemy.hpp>
#include "EnemyCWPhases\EnemyCWPhase0.hpp"

class BDEnemyCW : public BTEnemy {
public:

	BDEnemyCW();
	~BDEnemyCW() final;

	void Start() override;

private:

	// phases
	EnemyCWPhase0* ph0;

	// components
	SpriteRenderer* spr;

	RTTR_ENABLE(BTEnemy) RTTR_REGISTRATION_FRIEND
};

#endif // !BATTLE_DEMO_ENEMY_CW_HPP