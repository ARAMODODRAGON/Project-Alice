#ifndef ALICE_SCENES_BATTLELEVELS_DEMOBTL_HPP
#define ALICE_SCENES_BATTLELEVELS_DEMOBTL_HPP
#include "../BattleLevel.hpp"
#include <ALC\Content\Sound\SoundSystem.hpp>
#include "../../Enemies/RuiEnemy.hpp"
#include "../../Systems/BulletDeleterSystem.hpp"
#include "../../Systems/Bullet Types/HomingBulletSystem.hpp"

class Battle1 final : public BattleLevel {
public:

	Battle1();
	~Battle1();

private:
	void Init() override;
	void Exit() override;
	void GameStep(ALC::Timestep ts) override;
	void Step(ALC::Timestep ts) override;

	ALC::string m_musicFile;
	DialogueHandler m_beginLevel;
	ALC::EntityID m_enemy;
	RuiEnemy* m_enemyBehavior;
	BulletDeleterSystem m_deleter;
	HomingBulletSystem m_homingsystem;

};

#endif // !ALICE_SCENES_BATTLELEVELS_DEMOBTL_HPP