#include "BattleLevel.hpp"
#include "../Enemies/Enemy.hpp"
#include "../Characters/Character.hpp"

#define DEBUG_FONT_SIZE 18
#define DEBUG_FPS_UPDATE_RATE 5

BattleLevel::BattleLevel()
	: m_timescale(1.0f), m_character(nullptr), m_debug(_DEBUG), m_lastFPS(0.0f), m_delta(0.0), m_counter(-1) {
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
	using CM = ALC::ContentManager;
	// load the font and make sure its not tied to a context
	m_debugFont = CM::LoadFont(CM::Default(), "Resources/Fonts/arial.ttf", DEBUG_FONT_SIZE);

	// set our content storage as the context
	ALC::ContentManager::SetContext(m_storage);

	// setup camera 
	// dunno why [* 0.3f] makes things work properly but yea cool...
	m_camera.SetCameraSize(BattleManager::PrefferedResolution());

	// load all textures for the UI (Spells, hearts, etc.)
	auto UIElements = ALC::ContentManager::LoadTexture(GetStorage(), "Resources/Textures/Battle UI & Backgrounds/UI_Textures.png");
	SetUIElements(UIElements);

	// create the character
	if (!m_character) m_character = BattleManager::InitAsCharacter(m_reg.Create());

}

void BattleLevel::Exit() { 
	// unset the player selection
	BattleManager::SetCharacter(CharaType::None);
}

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

	// Drawing the player's current HP in hearts, their current spells, and the position of the enemy on the x-axis
	if (m_UIElements) {
		ALC::rect pos(0.0f);
		pos.min = ALC::vec2(980.0f, 125.0f);
		pos.max = pos.min + ALC::vec2(45.0f, 40.0f);

		ALC::rect target(0.0f);
		target = BattleManager::GetCurrentCharacter()->GetHeartTargetRect();

		// Display the player's current health and max health in the form of hearts
		uint32_t health = (uint32_t)glm::ceil(BattleManager::GetCurrentCharacter()->GetHealth());
		uint32_t maxHealth = (uint32_t)glm::ceil(BattleManager::GetCurrentCharacter()->GetMaxHealth());
		for (uint32_t i = 0; i < maxHealth; i++) {
			if (health > i) { m_ui.DrawQuad(pos, ALC_COLOR_WHITE, target, m_UIElements); }
			else { m_ui.DrawQuad(pos, ALC_COLOR_BLACK, target, m_UIElements); }
			pos.min.x += 45.0f;
			pos.max.x += 45.0f;
		}

		// Displaying the player's currently equipped attack spell
		pos.min = ALC::vec2(990.0f, 176.0f);
		pos.max = pos.min + ALC::vec2(120.0f, 120.0f);

		target = BattleManager::GetCurrentCharacter()->GetAttackTargetRect();

		m_ui.DrawQuad(pos, ALC_COLOR_WHITE, target, m_UIElements);

		// Displaying the player's currently equipped defence spell
		pos.min.x += 131.0f;
		pos.max.x += 131.0f;

		target = BattleManager::GetCurrentCharacter()->GetDefenceTargetRect();
		ALC::rect cooldown = BattleManager::GetCurrentCharacter()->GetDefenceTargetRectCooldown();
		if (cooldown.max.y > 0.0f) { // Darken the image to show the cooldown timer
			m_ui.DrawQuad(pos, ALC::vec4(0.25f, 0.25f, 0.25f, 1.0f), target, m_UIElements);
			pos.min.y = pos.max.y - (cooldown.Height() * 5);
			m_ui.DrawQuad(pos, ALC_COLOR_WHITE, cooldown, m_UIElements);
		} else { // Display the image as normal
			m_ui.DrawQuad(pos, ALC_COLOR_WHITE, target, m_UIElements);
		}

		// Finally, display the tracker at the bottom of the screen to show the enemy's horizontal position on the screen
		ALC::Transform2D& enemyTransform = BattleManager::GetEnemy()->GetEntity().GetComponent<ALC::Transform2D>();
		ALC::mat4 worldToScreen = m_camera.GetWorldToScreen();
		float enemyX = ALC::vec2(worldToScreen * ALC::vec4(enemyTransform.position, 0.0f, 1.0f)).x;

		pos.min = ALC::vec2(enemyX - 26.0f, 700.0f);
		pos.max = ALC::vec2(enemyX + 26.0f, 716.0f);

		target = ALC::rect(97.0f, 28.0f, 110.0f, 32.0f);

		m_ui.DrawQuad(pos, ALC_COLOR_WHITE, target, m_UIElements);
	}

	// draw debug stuff like fps
	if (m_debug) {
		m_ui.DrawText("Current FPS: " + VTOS((int)m_lastFPS), m_debugFont, ALC::vec2(0.0f, DEBUG_FONT_SIZE));
		m_ui.DrawText("Delta Time: " + VTOS(m_delta), m_debugFont, ALC::vec2(0.0f, (DEBUG_FONT_SIZE * 2) + 2));
		m_ui.DrawText("Timescale: " + VTOS(m_timescale) + " (Target FPS is " + VTOS((int)(60.0f * m_timescale)) + ")", m_debugFont, ALC::vec2(0.0f, (DEBUG_FONT_SIZE * 3) + 4));

		m_ui.DrawText("Total Entities: " + VTOS(GetReg().__GetReg().size<ALC::EntityInfo>()), m_debugFont, ALC::vec2(0.0f, (DEBUG_FONT_SIZE * 5) + 8));
		m_ui.DrawText("Enemy Health: " + VTOS((int)BattleManager::GetEnemy()->GetHealth()) + " / " + VTOS((int)BattleManager::GetEnemy()->GetMaxHealth()), m_debugFont, ALC::vec2(0.0f, (DEBUG_FONT_SIZE * 6) + 10));
	}

	// end drawing UI
	m_ui.End();

}

void BattleLevel::Step(ALC::Timestep t) {
	++m_counter;

	using ALC::Keyboard;
	using ALC::KeyCode;

	if ((m_counter % DEBUG_FPS_UPDATE_RATE) == 0)
		m_lastFPS = t.GetFPS();

	m_delta = t.GetDouble();

	auto ctrl = Keyboard::GetKey(KeyCode::LeftCtrl);
	auto keyw = Keyboard::GetKey(KeyCode::KeyW);
	if ((ctrl && keyw.Pressed()) || (ctrl.Pressed() && keyw)) {
		m_debug = !m_debug;
	}

	// prepare our fixed timestep
	ALC::Timestep fixedts((1.0 / 60.0) * m_timescale);

	// call gamestep
	GameStep(fixedts);

	// update behaviors
	m_reg.UpdateBehaviors(fixedts);

	// update physics
	m_bPhysics.Step(m_reg, fixedts);

	// create all entities
	m_ech.Cleanup(m_reg);

	// late update the registry
	m_reg.LateUpdateBehaviors(fixedts);

}
