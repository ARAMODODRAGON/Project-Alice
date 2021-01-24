#include "BattleLevel.hpp"

BattleLevel::BattleLevel()
	: m_timescale(1.0f), m_character(nullptr), m_debug(_DEBUG) {
	m_ui.SetInternalScreenSize(BattleManager::PrefferedResolution());
}

BattleLevel::~BattleLevel() { }

ALC::rect BattleLevel::GetScreenLevelBounds() const {
	return GetScreenLevelBounds(BattleManager::PrefferedResolution());
}

ALC::rect BattleLevel::GetScreenLevelBounds(const ALC::vec2& screensize) const {
	// get level boundry in world coordinates
	ALC::mat4 worldToScreen = m_camera.GetWorldToScreen(screensize);
	ALC::rect levelbounds = BattleManager::GetLevelBounds();

	// convert to screen coordinates
	levelbounds.min = ALC::vec2(worldToScreen * ALC::vec4(levelbounds.min, 0.0f, 1.0f));
	levelbounds.max = ALC::vec2(worldToScreen * ALC::vec4(levelbounds.max, 0.0f, 1.0f));

	return levelbounds;
}

void BattleLevel::Init() {

	// set our content storage as the context
	ALC::ContentManager::SetContext(m_storage);

	// setup camera 
	// dunno why [* 0.3f] makes things work properly but yea cool...
	m_camera.SetCameraSize(BattleManager::PrefferedResolution());

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

	// draw debug level boundries
	if (m_debug) {
		ALC::rect levelbounds = GetScreenLevelBounds();

		// draw the level boundries
		ALC::rect r;
		constexpr float borderWidth = 10.0f;
		// left
		r.bottom = levelbounds.bottom - borderWidth;
		r.top = levelbounds.top + borderWidth;
		r.right = levelbounds.left;
		r.left = r.right - borderWidth;
		m_ui.DrawQuad(r, ALC_COLOR_GREEN);

		// right
		r.bottom = levelbounds.bottom - borderWidth;
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

	// draw ui overlay but only if it has been set
	if (m_UIOverlay) {
		ALC::rect pos(0.0f);
		pos.max = m_ui.GetInternalScreenSize();

		ALC::rect target(0.0f);
		target.max = m_UIOverlay.GetSize();

		m_ui.DrawQuad(pos, ALC_COLOR_WHITE, target, m_UIOverlay);
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
