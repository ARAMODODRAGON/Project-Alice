#include "DemoBTL.hpp"
#include <ALC/Input.hpp>

DemoBTL::DemoBTL()
	: m_musicFile("Resources/Audio/Empty_Score.mp3")
	, m_beginLevel("Resources/Dialogues/TestDialogue.json", &GetStorage())
	, m_enemyBehavior(nullptr)
	, m_deleter(GetECH()) 
	, m_homingsystem(GetReg()) { 
	m_deleter.SetDeathBoundry(BattleManager::GetLevelBounds());
}

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

	// create enemy
	auto enemy = GetReg().Create();
	m_enemyBehavior = enemy.AddBehavior<RuiEnemy>();
	m_enemy = enemy.GetComponent<ALC::EntityInfo>().GetID();

	// target enemy
	m_homingsystem.AddTarget(m_enemy);
}

void DemoBTL::Exit() {
	BattleLevel::Exit();

	// stop and delete the music
	ALC::SoundSystem::PauseMusic();
	ALC::SoundSystem::UnloadMusic(m_musicFile);
}

void DemoBTL::GameStep(ALC::Timestep ts) { }

void DemoBTL::Step(ALC::Timestep ts) {
	BattleLevel::Step(ts);
	auto& reg = GetReg();

	reg.StepSystem(ts, m_homingsystem);

	reg.StepSystem(ts, m_deleter);
}
