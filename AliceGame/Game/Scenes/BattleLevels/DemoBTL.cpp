#include "DemoBTL.hpp"
#include <ALC/Input.hpp>

DemoBTL::DemoBTL()
	: m_musicFile("Resources/Audio/Empty_Score.mp3")
	, m_beginLevel("Resources/Dialogues/TestDialogue.json", &GetStorage())
	, m_enemyBehavior(nullptr) { }

DemoBTL::~DemoBTL() { }

void DemoBTL::Init() {

	// load our music
	if (ALC::SoundSystem::LoadMusic(m_musicFile)) {
		// play
		//ALC::SoundSystem::PlayMusic(m_musicFile);
	}

	// we want to spawn alice
	BattleManager::SetCharcter(CharaType::Alice);

	// alice will spawn here
	BattleLevel::Init();

	m_enemy = GetReg().Create();
	m_enemyBehavior = m_enemy.AddBehavior<RuiEnemy>();
}

void DemoBTL::Exit() {
	BattleLevel::Exit();

	// stop and delete the music
	ALC::SoundSystem::PauseMusic();
	ALC::SoundSystem::UnloadMusic(m_musicFile);
}

void DemoBTL::GameStep(ALC::Timestep t) { }
