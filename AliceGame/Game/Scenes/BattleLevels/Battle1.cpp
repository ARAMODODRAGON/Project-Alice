#include "Battle1.hpp"
#include <ALC/Input.hpp>
#include "../../Characters/Character.hpp"

Battle1::Battle1()
	: m_musicFile("Resources/Audio/First_Boss.mp3")
	, m_enemyBehavior(nullptr)
	, m_deleter(GetECH())
	, m_homingsystem(GetReg())
	, m_gravitySystem()
	, m_cirlceBombSystem()
	, m_timer(0.0f)
	, m_battleDone(false) {
	m_deleter.SetDeathBoundry(BattleManager::GetLevelBounds());
	m_gravitySystem.SetGravity(ALC::vec2(0.0f, -40.0f));
}

Battle1::~Battle1() { }

void Battle1::Init() {

	// load all textures
	auto UIOverlay = ALC::ContentManager::LoadTexture(GetStorage(), "Resources/Textures/UI/Rui_Battle_UI.png");
	SetUIOverlay(UIOverlay);
	m_background = ALC::ContentManager::LoadTexture(GetStorage(), "Resources/Textures/UI/Rui_Battle_Background_4.png");

	// load our music
	if (ALC::SoundSystem::LoadMusic(m_musicFile)) {
		// play
		ALC::SoundSystem::PlayMusic(m_musicFile);
		//ALC::SoundSystem::PauseMusic();
	}

	// we want to spawn alice
	BattleManager::SetCharacter(CharaType::Alice);

	// alice will spawn here
	BattleLevel::Init();
	SetStartingDialogue("Resources/Dialogues/TestDialogue.json");

	// create enemy
	auto enemy = GetReg().Create();
	m_enemyBehavior = enemy.AddBehavior<RuiEnemy>();
	//m_enemyBehavior->BattleBegin();
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
	auto& jobap = GetJobs();

	jobap.ApplyJobs(ts, m_homingsystem);
	jobap.ApplyJobs(ts, m_gravitySystem);
	jobap.ApplyJobs(ts, m_cirlceBombSystem);
	reg.StepSystem(ts, m_deleter);

}

void Battle1::Step(ALC::Timestep ts) {
	BattleLevel::Step(ts);
	m_timer += ts;
	if (m_enemyBehavior->IsDone() && !m_battleDone) {
		ALC::SoundSystem::PauseMusic();
		SetStartingDialogue("Resources/Dialogues/TestDialogue.json");
		m_battleDone = true;
		BattleManager::GetCurrentCharacter()->BattleToggle();
	}
}

void Battle1::DrawBackground(ALC::UIBatch& ui) {
	if (m_background) {
		static const ALC::rect targets[] = {
			ALC::rect(0.0f,   0.0f, 639.0f, 639.0f),
			ALC::rect(0.0f, 640.0f, 639.0f, 639.0f + 640.0f),
			ALC::rect(0.0f, 639.0f + 640.0f, 639.0f, 639.0f + 640.0f * 2.0f),
			ALC::rect(0.0f, 640.0f * 3.0f, 639.0f, 639.0f + 640.0f * 3.0f)
		};
		static const ALC::vec2 dir[] = {
			glm::normalize(ALC::vec2(0.2f, 1.0f)),
			glm::normalize(ALC::vec2(1.0f, 1.0f)),
			glm::normalize(ALC::vec2(-0.5f, -1.0f)),
			glm::normalize(ALC::vec2(0.8f, 1.0f))
		};

		const ALC::vec2 screen = ui.GetInternalScreenSize();
		for (size_t i = 0; i < 4; i++) {
			ALC::rect pos(0.0f, 0.0f, screen.x, screen.x);
			if (i != 3) {
				float offsetX = sinf(m_timer) * 30.0f * dir[i].x;
				float offsetY = sinf(m_timer + 0.5f) * 30.0f * dir[i].y;
				//offsetX = round(offsetX * 0.1f) * 10.0f;
				//offsetY = round(offsetY * 0.1f) * 10.0f;
				pos.left += offsetX;
				pos.right += offsetX;
				pos.top += offsetY;
				pos.bottom += offsetY;
			}
			ALC::vec4 color = ALC_COLOR_WHITE * 0.9f;
			if (i != 0) color.a = ((sinf(m_timer + (0.3f * float(i))) + 1.0f) * 0.5f) * 0.5f + 0.2f;

			ui.DrawQuad(pos, color, targets[i], m_background);
		}
	}
	//if (m_background) {
	//	// cant use constexpr so we do this
	//	static const ALC::rect target(0.0f, 0.0f, 127.0f, 127.0f);
	//
	//	const ALC::vec2 screen = ui.GetInternalScreenSize();
	//	ALC::rect pos(0.0f, 0.0f, screen.x, screen.x);
	//	float offsetX = sin(m_timer * 0.01f) * 50.0f;
	//	offsetX = roundf(offsetX * 0.1f) * 10.0f;
	//	float offsetY = sin(m_timer * 0.1f) * 30.0f;
	//	offsetY = roundf(offsetY * 0.1f) * 10.0f;
	//	pos.left += offsetX;
	//	pos.right += offsetX;
	//	pos.top += offsetY;
	//	pos.bottom += offsetY;
	//	ALC::vec4 color = ALC_COLOR_WHITE;
	//	color.a = ((sinf(m_timer) + 1.0f) * 0.5f) * 0.5f + 0.2f;
	//
	//	ui.DrawQuad(pos, color, target, m_background);
	//
	//}
	//if (m_background) {
	//	// cant use constexpr so we do this
	//	static const ALC::rect rectA(0.0f, 0.0f, 127.0f, 127.0f);
	//	static const ALC::rect rectB(128.0f, 0.0f, 255.0f, 127.0f);
	//	static const ALC::rect rectC(256.0f, 0.0f, 383.0f, 127.0f);
	//
	//	const ALC::vec2 screen = ui.GetInternalScreenSize();
	//	ALC::rect pos(0.0f, 0.0f, screen.x, screen.x);
	//
	//	ALC::vec4 color = ALC_COLOR_WHITE;
	//
	//	{
	//		auto r = pos;
	//		float offset = round((sinf(m_timer * 1.5f) * 30.0f) * 0.1f) * 10.0f;
	//		r.left += offset;
	//		r.right -= offset;
	//		r.top -= offset;
	//		r.bottom += offset;
	//		ui.DrawQuad(r, color, rectA, m_background);
	//	}
	//	{
	//		color.a = 0.5f;
	//		auto r = pos;
	//		float offset = -round((sinf(m_timer * 1.5f) * 30.0f) * 0.1f) * 10.0f;
	//		r.left += offset;
	//		r.right -= offset;
	//		r.top -= offset;
	//		r.bottom += offset;
	//		ui.DrawQuad(r, color, rectB, m_background);
	//	}
	//	color.a = 0.5f;
	//	ui.DrawQuad(pos, color, rectC, m_background);
	//
	//}
}
