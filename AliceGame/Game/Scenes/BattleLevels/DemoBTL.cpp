#include "DemoBTL.hpp"

DemoBTL::DemoBTL() { }

DemoBTL::~DemoBTL() { }

void DemoBTL::Init() { 

	// we want to spawn alice
	BattleManager::SetCharcter(CharaType::Alice);

	// alice will spawn here
	BattleLevel::Init();
}

void DemoBTL::Exit() { 
	BattleLevel::Exit();
}

void DemoBTL::GameStep(ALC::Timestep t) { }
