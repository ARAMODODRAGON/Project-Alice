#include "Battle1.hpp"
#include <ALC/Input.hpp>

Battle1::Battle1()
	: m_musicFile("Resources/Audio/Empty_Score.mp3")
	, m_beginLevel("Resources/Dialogues/TestDialogue.json", &GetStorage())
	, m_enemyBehavior(nullptr)
	, m_deleter(GetECH())
	, m_homingsystem(GetReg())
	, m_backgroundPos(0.0f)
	, m_scrollspeed(5.0f) {
	m_deleter.SetDeathBoundry(BattleManager::GetLevelBounds());
}

Battle1::~Battle1() { }

void Battle1::Init() {

	// load all textures
	auto UIOverlay = ALC::ContentManager::LoadTexture(GetStorage(), "Resources/Textures/Battle UI & Backgrounds/Rui_Battle_UI.png");
	SetUIOverlay(UIOverlay);
	m_background = ALC::ContentManager::LoadTexture(GetStorage(), "Resources/Textures/Battle UI & Backgrounds/Rui_Battle_Background.png");

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

	// move background
	m_backgroundPos += m_scrollspeed * ts;
	const float maxpos = float(m_background.GetSize().y - 66);
	if (m_backgroundPos >= maxpos)
		m_backgroundPos -= maxpos;
}

void Battle1::PreDraw() {
	BattleLevel::PreDraw();
	auto& ui = GetUI();
	ui.Begin();

	ALC::rect pos(350.0f, 30.0f, 930.0f, 690.0f);

	ALC::vec2 size = m_background.GetSize();
	ALC::rect target(0.0f, (size.y - 66.0f) - m_backgroundPos,
					 size.x, size.y - m_backgroundPos);

	ui.DrawQuad(pos, ALC_COLOR_WHITE, target, m_background);

	ui.End();
}

