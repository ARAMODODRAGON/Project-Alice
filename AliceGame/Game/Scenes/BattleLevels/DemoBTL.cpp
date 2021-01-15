#include "DemoBTL.hpp"

DemoBTL::DemoBTL() : m_musicFile("Resources/Audio/Empty_Score.mp3"){ }

DemoBTL::~DemoBTL() { }

void DemoBTL::Init() { 

	// load our music
	if (ALC::SoundSystem::LoadMusic(m_musicFile)) {
		// play
		ALC::SoundSystem::PlayMusic(m_musicFile);
	}


	// we want to spawn alice
	BattleManager::SetCharcter(CharaType::Alice);

	// alice will spawn here
	BattleLevel::Init();
}

void DemoBTL::Exit() { 
	BattleLevel::Exit();
}

void DemoBTL::GameStep(ALC::Timestep t) { }
