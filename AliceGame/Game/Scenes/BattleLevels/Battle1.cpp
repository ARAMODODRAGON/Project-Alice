#include "Battle1.hpp"
#include <ALC/Input.hpp>

Battle1::Battle1()
	: m_musicFile("Resources/Audio/Empty_Score.mp3")
	, m_beginLevel("Resources/Dialogues/TestDialogue.json", &GetStorage())
	, m_enemyBehavior(nullptr)
	, m_deleter(GetECH())
	, m_homingsystem(GetReg()) {
	m_deleter.SetDeathBoundry(BattleManager::GetLevelBounds());
}

Battle1::~Battle1() { }

void Battle1::Init() {

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
	m_enemyBehavior->BattleBegin();
	m_enemy = enemy.GetComponent<ALC::EntityInfo>().GetID();

	// target enemy
	m_homingsystem.AddTarget(m_enemy);
}

void Battle1::Exit() {
	BattleLevel::Exit();

	// stop and delete the music
	ALC::SoundSystem::PauseMusic();
	ALC::SoundSystem::UnloadMusic(m_musicFile);
}

void Battle1::GameStep(ALC::Timestep ts) { }

void Battle1::Step(ALC::Timestep ts) {
	BattleLevel::Step(ts);
	auto& reg = GetReg();

	reg.StepSystem(ts, m_homingsystem);

	reg.StepSystem(ts, m_deleter);
}
