#include "Battle1.hpp"
#include <ALC/Input.hpp>

Battle1::Battle1()
	: m_musicFile("Resources/Audio/Empty_Score.mp3")
	, m_beginLevel("Resources/Dialogues/TestDialogue.json", &GetStorage())
	, m_enemyBehavior(nullptr)
	, m_deleter(GetECH())
	, m_homingsystem(GetReg())
	, m_gravitySystem()
	, m_cirlceBombSystem()
	, m_ZzBulletSystem()
	, m_timer(0.0f) {
	m_deleter.SetDeathBoundry(BattleManager::GetLevelBounds());
	m_gravitySystem.SetGravity(ALC::vec2(0.0f, -40.0f));
}

Battle1::~Battle1() { }

void Battle1::Init() {

	// load all textures
	auto UIOverlay = ALC::ContentManager::LoadTexture(GetStorage(), "Resources/Textures/UI/Rui_Battle_UI.png");
	SetUIOverlay(UIOverlay);
	m_background = ALC::ContentManager::LoadTexture(GetStorage(), "Resources/Textures/UI/Rui_Battle_Background_UI-sheet.png");

	// load our music
	if (ALC::SoundSystem::LoadMusic(m_musicFile)) {
		// play
		//ALC::SoundSystem::PlayMusic(m_musicFile);
	}

	// we want to spawn alice
	BattleManager::SetCharacter(CharaType::Alice);

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

void Battle1::GameStep(ALC::Timestep ts) {
	auto& reg = GetReg();

	reg.StepSystem(ts, m_homingsystem);
	reg.StepSystem(ts, m_gravitySystem);
	reg.StepSystem(ts, m_cirlceBombSystem);
	reg.StepSystem(ts, m_ZzBulletSystem);
	reg.StepSystem(ts, m_deleter);
}

void Battle1::Step(ALC::Timestep ts) {
	BattleLevel::Step(ts);
	m_timer += ts;
}

void Battle1::DrawBackground(ALC::UIBatch& ui) {
	if (m_background) {
		//// cant use constexpr so we do this
		//static const ALC::rect rectA(0.0f, 0.0f, 127.0f, 127.0f);
		//static const ALC::rect rectB(128.0f, 0.0f, 255.0f, 127.0f);
		//static const ALC::rect rectC(256.0f, 0.0f, 383.0f, 127.0f);
		//
		//const ALC::vec2 screen = ui.GetInternalScreenSize();
		//ALC::rect pos(0.0f, 0.0f, screen.x, screen.x);
		//
		//ALC::vec4 color = ALC_COLOR_WHITE;
		//ui.DrawQuad(pos, color, rectA, m_background);
		//color.a = 0.5f;
		//ui.DrawQuad(pos, color, rectB, m_background);
		//color.a = 0.5f;
		//ui.DrawQuad(pos, color, rectC, m_background);

	}
}
