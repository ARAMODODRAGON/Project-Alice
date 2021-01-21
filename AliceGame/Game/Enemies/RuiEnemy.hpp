#ifndef ALICE_ENEMIES_RUIENEMY_HPP
#define ALICE_ENEMIES_RUIENEMY_HPP
#include "Enemy.hpp"

class RuiEnemy final : public Enemy {
public:

	RuiEnemy();
	~RuiEnemy();

	void Start(ALC::Entity self) override;
	void Update(ALC::Entity self, ALC::Timestep ts) override;
	void LateUpdate(ALC::Entity self, ALC::Timestep ts) override;
	void OnDeath(ALC::Entity self) override;
	void BattleBegin() override;

};

#endif // !ALICE_ENEMIES_RUIENEMY_HPP