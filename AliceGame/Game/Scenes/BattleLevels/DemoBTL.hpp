#ifndef ALICE_SCENES_BATTLELEVELS_DEMOBTL_HPP
#define ALICE_SCENES_BATTLELEVELS_DEMOBTL_HPP
#include "../BattleLevel.hpp"

class DemoBTL final : public BattleLevel {
public:

	DemoBTL();
	~DemoBTL();

private:
	void Init() override;
	void Exit() override;
	void GameStep(ALC::Timestep t) override;
};

#endif // !ALICE_SCENES_BATTLELEVELS_DEMOBTL_HPP