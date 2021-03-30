#ifndef ALICE_ENEMIES_EDENENEMY_HPP
#define ALICE_ENEMIES_EDENENEMY_HPP
#include "Enemy.hpp"

class EdenEnemy final : public Enemy {
public:

	EdenEnemy();
	~EdenEnemy();

	void Start(ALC::Entity self) override;
	void Update(ALC::Entity self, ALC::Timestep ts) override;
	void LateUpdate(ALC::Entity self, ALC::Timestep ts) override;
	void OnDeath(ALC::Entity self) override;
	void BattleToggle() override;

private:

};

#endif // !ALICE_ENEMIES_EDENENEMY_HPP