#ifndef ALICE_SCENES_BATTLELEVELS_DEMOBTL_HPP
#define ALICE_SCENES_BATTLELEVELS_DEMOBTL_HPP
#include "../BattleLevel.hpp"
#include <ALC\Content\Sound\SoundSystem.hpp>
#include "../../Enemies/RuiEnemy.hpp"

class DemoBTL final : public BattleLevel {
public:

	DemoBTL();
	~DemoBTL();

private:
	void Init() override;
	void Exit() override;
	void GameStep(ALC::Timestep t) override;

	ALC::string m_musicFile;
	DialogueHandler m_beginLevel;
	ALC::Entity m_enemy;
	RuiEnemy* m_enemyBehavior;
};

#endif // !ALICE_SCENES_BATTLELEVELS_DEMOBTL_HPP