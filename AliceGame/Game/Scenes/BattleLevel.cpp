#include "BattleLevel.hpp"

BattleLevel::BattleLevel() : m_timescale(1.0f), m_character(nullptr) { }

BattleLevel::~BattleLevel() { }

void BattleLevel::Init() {

	// set our content storage as the context
	ALC::ContentManager::SetContext(m_storage);

	// setup camera
	m_camera.SetCameraSize(m_camera.GetCameraSize() * 0.3f);

	// create the character
	if (!m_character) m_character = BattleManager::InitAsCharacter(m_reg.Create());

}

void BattleLevel::Exit() { }

void BattleLevel::Draw() {

	// draw sprites
	m_batch.Begin(m_camera);
	m_batch.DrawComponents(m_reg);
	m_batch.End();

	// draw ui
	m_ui.Begin();

	// get level boundry in world coordinates
	ALC::mat4 worldToScreen = m_camera.GetWorldToScreen();
	ALC::rect levelbounds = BattleManager::GetLevelBounds();

	// convert to screen coordinates
	levelbounds.min = ALC::vec2(worldToScreen * ALC::vec4(levelbounds.min, 0.0f, 1.0f));
	levelbounds.max = ALC::vec2(worldToScreen * ALC::vec4(levelbounds.max, 0.0f, 1.0f));

	// draw the level boundries
	ALC::rect r;
	constexpr float borderWidth = 10.0f;
	{
		// left
		r.bottom = levelbounds.bottom - borderWidth;
		r.top = levelbounds.top + borderWidth;
		r.right = levelbounds.left;
		r.left = r.right - borderWidth;
		m_ui.DrawQuad(r, ALC_COLOR_GREEN);

		// right
		r.bottom = levelbounds.bottom - borderWidth ;
		r.top = levelbounds.top + borderWidth;
		r.left = levelbounds.right;
		r.right = r.left + borderWidth;
		m_ui.DrawQuad(r, ALC_COLOR_GREEN);

		// top
		r.left = levelbounds.left;
		r.right = levelbounds.right;
		r.bottom = levelbounds.top;
		r.top = r.bottom + borderWidth;
		m_ui.DrawQuad(r, ALC_COLOR_GREEN);

		// bottom
		r.left = levelbounds.left;
		r.right = levelbounds.right;
		r.top = levelbounds.bottom;
		r.bottom = r.top - borderWidth;
		m_ui.DrawQuad(r, ALC_COLOR_GREEN);
	}

	// end drawing UI
	m_ui.End();

}

void BattleLevel::Step(ALC::Timestep t) {
	// prepare our fixed timestep
	ALC::Timestep fixedts((1.0 / 60.0) * m_timescale);

	// call gamestep
	GameStep(fixedts);

	// update behaviors
	m_reg.UpdateBehaviors(t);

	// update physics
	m_bPhysics.Step(m_reg, t);

	// create all entities
	m_ech.Cleanup(m_reg);

	// late update the registry
	m_reg.LateUpdateBehaviors(fixedts);

}
