#ifndef ALICE_SCENES_BATTLELEVELS_BATTLE2_HPP
#define ALICE_SCENES_BATTLELEVELS_BATTLE2_HPP
#include "../BattleLevel.hpp"
#include "../../Enemies/EdenEnemy.hpp"
#include "../../Systems/BulletDeleterSystem.hpp"
#include "../../Systems/Bullet Types/HomingBulletSystem.hpp"

class Battle2 final : public BattleLevel {
public:

	Battle2();
	~Battle2();

	void Init() override;
	void Exit() override;
	void GameStep(ALC::Timestep t) override;
	void Draw() override;
	void DrawBackground(ALC::UIBatch& ui) override { }

private:

	EdenEnemy* m_enemy;

	// systems
	BulletDeleterSystem m_deletersys;
	HomingBulletSystem m_homingsys;

};

#endif // !ALICE_SCENES_BATTLELEVELS_BATTLE2_HPP