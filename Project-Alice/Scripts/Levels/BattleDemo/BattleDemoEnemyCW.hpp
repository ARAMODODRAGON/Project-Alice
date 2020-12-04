#ifndef BATTLE_DEMO_ENEMY_CW_HPP
#define BATTLE_DEMO_ENEMY_CW_HPP
#include <Engine\Battle\BTEnemy.hpp>
#include "EnemyCWPhases\EnemyCWPhase0.hpp"
#include "EnemyCWPhases\EnemyCWPhase1.hpp"
#include "EnemyCWPhases\EnemyCWPhase2.hpp"

class BDEnemyCW : public BTEnemy {
public:

	BDEnemyCW();
	~BDEnemyCW() final;

	void Start() override;
	void Update() override;

private:

	// phases
	EnemyCWPhase0* ph0;
	EnemyCWPhase1* ph1;
	EnemyCWPhase2* ph2;

	// components
	SpriteRenderer* spr;

	RTTR_ENABLE(BTEnemy) RTTR_REGISTRATION_FRIEND
};

#endif // !BATTLE_DEMO_ENEMY_CW_HPP