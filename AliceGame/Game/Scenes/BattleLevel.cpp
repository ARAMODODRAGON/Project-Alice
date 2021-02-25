#include "BattleLevel.hpp"
#include "../Enemies/Enemy.hpp"
#include "../Characters/Character.hpp"

#define DEBUG_FONT_SIZE 18
#define DEBUG_FPS_UPDATE_RATE 5

BattleLevel::BattleLevel()
	: m_timescale(1.0f)
	, m_character(nullptr)
	, m_debug(_DEBUG)
	, m_lastFPS(0.0f)
	, m_delta(0.0)
	, m_itemsOffset(20.0f)
	, m_itemFillspeed(0.1f)
	, m_pauseSelected(0)
	, m_counter(-1)
	, m_playerWasDead(false)
	, m_isPaused(false)
	, m_isFading(true)
	, m_shouldFadeIn(true)
	, m_fadeTransition(0.0f)
	, m_fadeMaxTransitionTime(1.0f)
	, m_pauseTransition(0.0f)
	, m_pauseMaxTransitionTime(0.3f)
	, m_reloadDelay(0.1f) {
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

	if (m_debugFont) {
		ALC::string testString("This is a test to see if the string will be split into multiple lines! This will probably hopefully be multiple lines!");

		multiLineText = m_debugFont.StringSplitLines(testString, 240.0f);
		stringDimensions = m_debugFont.StringDimensions(multiLineText);
	}

	// set our content storage as the context
	ALC::ContentManager::SetContext(m_storage);

	// setup camera 
	m_camera.SetCameraSize(BattleManager::PrefferedResolution());

	// load all textures for the UI (Spells, hearts, etc.)
	auto UIElements = ALC::ContentManager::LoadTexture(GetStorage(), "Resources/Textures/Battle UI & Backgrounds/UI_Textures.png");
	SetUIElements(UIElements);

	// create the character
	if (!m_character) m_character = BattleManager::InitAsCharacter(m_reg.Create());

	// setup the pause menu stuff
	ALC::vec2 screenCenter = BattleManager::PrefferedResolution() / 2.0f;

	auto PauseFont = CM::LoadFont(CM::Default(), "Resources/Fonts/Roboto-BoldItalic.ttf", 50);
	auto SelectionFont = CM::LoadFont(CM::Default(), "Resources/Fonts/Roboto-Medium.ttf", 20);

	m_itemPaused.text = "Paused";
	m_itemPaused.position = screenCenter + ALC::vec2(0.0f, -100.0f);
	m_itemPaused.font = PauseFont;
	m_itemPaused.dimensions = m_itemPaused.font.StringDimensions(m_itemPaused.text);
	m_itemPaused.position.x -= m_itemPaused.dimensions.x * 0.5f;

	auto& continueItem = m_pauseSelection[0];
	continueItem.text = "Continue";
	continueItem.position = m_itemPaused.position;
	continueItem.position.y += m_itemPaused.dimensions.y + m_itemsOffset;
	continueItem.font = SelectionFont;
	continueItem.dimensions = continueItem.font.StringDimensions(continueItem.text);
	continueItem.func = &BattleLevel::OnContinue;

	auto& RestartItem = m_pauseSelection[1];
	RestartItem.text = "Restart";
	RestartItem.position = continueItem.position;
	RestartItem.position.y += continueItem.dimensions.y + m_itemsOffset;
	RestartItem.font = SelectionFont;
	RestartItem.dimensions = RestartItem.font.StringDimensions(RestartItem.text);
	RestartItem.func = &BattleLevel::OnRestart;

}

void BattleLevel::Exit() {
	// unset the player selection
	BattleManager::SetCharacter(CharaType::None);
}

void BattleLevel::Draw() {

	// draw sprites
	m_batch.Begin(m_camera);

	// draw all SPRL layers
	for (ALC::int32 i = 0; i < SPRL::_COUNT; i++) {
		m_reg.ForeachComponent<ALC::Transform2D, ALC::SpriteComponent>(
			[this, i](auto entity, auto tr, auto spr) {
			if (spr.layer == i) {
				if (entity.HasComponent<ALC::BulletBody>()) {
					if (!entity.GetComponent<ALC::BulletBody>().isSimulated)
						return;
				}
				m_batch.Draw(tr, spr);
			}
		});
	}

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
			if (health > i) { m_ui.DrawQuad(pos, ALC_COLOR_WHITE, target, m_UIElements); } else { m_ui.DrawQuad(pos, ALC_COLOR_BLACK, target, m_UIElements); }
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
		ALC::mat4 worldToScreen = m_camera.GetWorldToScreen(m_ui.GetInternalScreenSize());
		float enemyX = ALC::vec2(worldToScreen * ALC::vec4(enemyTransform.position, 0.0f, 1.0f)).x;

		pos.min = ALC::vec2(enemyX - 26.0f, 700.0f);
		pos.max = ALC::vec2(enemyX + 26.0f, 716.0f);

		target = ALC::rect(97.0f, 28.0f, 110.0f, 32.0f);

		m_ui.DrawQuad(pos, ALC_COLOR_WHITE, target, m_UIElements);

		// draw pause menu
		if (m_isPaused) {

			// cover the screen with a semi transparent black texture to darken the background
			const float alpha = (m_pauseTransition / m_pauseMaxTransitionTime) * 0.8f;
			ALC::rect quad(ALC::vec2(0.0f), m_ui.GetInternalScreenSize());
			m_ui.DrawQuad(quad, ALC::vec4(0.0f, 0.0f, 0.0f, alpha));

			constexpr float margin = 7.0f;

			// draw text
			m_ui.DrawText(m_itemPaused.text, m_itemPaused.font, m_itemPaused.position);
			for (size_t i = 0; i < m_pauseSelection.size(); ++i) {
				UIItem& item = m_pauseSelection[i];

				// selection box
				quad.min = item.position;
				quad.max = quad.min;
				quad.min.y -= item.font.GetSize().y;
				quad.max.x += item.dimensions.x;

				quad.min.x -= margin;
				quad.min.y -= (margin - 6.0f);
				quad.max.x += margin;
				quad.max.y += margin;

				quad.max.x = (quad.max.x - quad.min.x) * (item.fillamount / m_itemFillspeed) + quad.min.x;

				m_ui.DrawQuad(quad, ALC::vec4(ALC::vec3(0.5f), 1.0f));

				// text
				m_ui.DrawText(item.text, item.font, item.position);
			}

		}

		// draw start fade
		if (m_isFading) {
			// cover the screen with a semi transparent black texture
			const float alpha = 1.0f - (m_fadeTransition / m_fadeMaxTransitionTime);
			ALC::rect quad(ALC::vec2(0.0f), m_ui.GetInternalScreenSize());
			m_ui.DrawQuad(quad, ALC::vec4(0.0f, 0.0f, 0.0f, alpha));
		}

	}

	// draw debug stuff like fps
	if (m_debug) {
		m_ui.DrawText("Current FPS: " + VTOS((int)m_lastFPS) // SINGLE CALL NOW WOOOOOOOOOOOOOOOOO
					  + "\nDelta Time : " + VTOS(m_delta)
					  + "\nTimescale: " + VTOS(m_timescale) + " (Target FPS is " + VTOS((int)(60.0f * m_timescale)) + ")"
					  + "\n\nTotal Entities: " + VTOS(GetReg().__GetReg().size<ALC::EntityInfo>())
					  + "\nEnemy Health: " + VTOS((int)BattleManager::GetEnemy()->GetHealth()) + " / " + VTOS((int)BattleManager::GetEnemy()->GetMaxHealth())
					  , m_debugFont, ALC::vec2(0.0f, 0.0f));

		ALC::rect r;
		r.min = ALC::vec2(0.0f, 204.0f);
		r.max = r.min + stringDimensions;
		m_ui.DrawQuad(r, ALC_COLOR_BLUE);

		m_ui.DrawText(multiLineText, m_debugFont, ALC::vec2(0.0f, 200.0f), ALC::vec4(1.0f));
	}

	// end drawing UI
	m_ui.End();

}

void BattleLevel::Step(ALC::Timestep ts) {
	++m_counter;

	using ALC::Keyboard;
	using ALC::KeyCode;

	if ((m_counter % DEBUG_FPS_UPDATE_RATE) == 0)
		m_lastFPS = ts.GetFPS();

	m_delta = ts.GetDouble();

	auto ctrl = Keyboard::GetKey(KeyCode::LeftCtrl);
	auto keyw = Keyboard::GetKey(KeyCode::KeyW);
	if ((ctrl && keyw.Pressed()) || (ctrl.Pressed() && keyw)) {
		m_debug = !m_debug;
	}

	// fade in
	if (m_isFading) {
		if (m_shouldFadeIn) {
			m_fadeTransition += ts;
			if (m_fadeTransition > m_fadeMaxTransitionTime) {
				m_isFading = false;
				m_fadeTransition = m_fadeMaxTransitionTime;
			}
		} else {
			m_fadeTransition -= ts;
			if (m_fadeTransition < 0.0f) {
				m_isFading = false;
				m_fadeTransition = 0.0f;
			}
		}
	}

	// pause / unpause
	if (!m_character->IsDead() && !m_isFading) {
		auto esc = Keyboard::GetKey(KeyCode::Escape);
		const bool canPause = ALC::NearlyZero(m_pauseTransition, ts) || ALC::NearlyEqual(m_pauseTransition, m_pauseMaxTransitionTime, ts);
		const bool wasPaused = m_isPaused;
		if (esc.Pressed() && canPause) m_isPaused = !m_isPaused;
		if (wasPaused && !m_isPaused) OnContinue();
	}
	if (m_isPaused) m_pauseTransition += ts;
	else			m_pauseTransition = 0.0f;
	m_pauseTransition = glm::clamp(m_pauseTransition, 0.0f, m_pauseMaxTransitionTime);

	// pause menu selection
	if (m_isPaused) {
		auto Up = Keyboard::GetKey(KeyCode::ArrowUp);
		auto Down = Keyboard::GetKey(KeyCode::ArrowDown);

		if (Up.Pressed() != Down.Pressed()) {
			if (Up.Pressed()) {
				m_pauseSelected--;
				m_pauseSelected %= m_pauseSelection.size();
			}
			if (Down.Pressed()) {
				m_pauseSelected++;
				m_pauseSelected %= m_pauseSelection.size();
			}
		}

		if (Keyboard::GetKey(KeyCode::KeyC).Pressed()) {
			auto func = m_pauseSelection[m_pauseSelected].func;
			(this->*func)();
		}

		for (size_t i = 0; i < m_pauseSelection.size(); i++) {
			auto& item = m_pauseSelection[i];
			if (i == m_pauseSelected)
				item.fillamount += ts;
			else
				item.fillamount -= ts;
			item.fillamount = glm::clamp(item.fillamount, 0.0f, m_itemFillspeed);
		}
	}

	// fade out
	if (m_character->IsDead()) {
		//m_timescale = 1.0f - (m_fadeTransition / m_fadeMaxTransitionTime);

		// reset scene
		if (ALC::NearlyZero(m_fadeTransition, ts)) {
			// TODO: built in reset functionality
			ALC::SceneManager::LoadLevel(ALC::SceneManager::GetActiveSceneBuildIndex()); // reset scene
		}
	}
	if (m_character->IsDead() != m_playerWasDead) {
		m_isFading = true;
		m_shouldFadeIn = false;
	}
	m_playerWasDead = m_character->IsDead();

	// prepare our fixed timestep. it should be 0 if paused
	ALC::Timestep fixedts((m_isPaused) ? 0.0 : ((1.0 / 60.0) * m_timescale));

	// call gamestep
	GameStep(fixedts);

	// dont update anything other than the level when fading
	if (!(m_isFading && m_shouldFadeIn) && !m_isPaused) { // tmp fix to make the death animation run
		// update behaviors
		m_reg.UpdateBehaviors(fixedts);

		// cleanup all entities
		m_ech.Cleanup(m_reg);

		// update physics
		m_bPhysics.Step(m_reg, fixedts);

		// late update the registry
		m_reg.LateUpdateBehaviors(fixedts);
	} else {
		// cleanup all entities
		m_ech.Cleanup(m_reg);
	}

}

void BattleLevel::OnContinue() {
	m_pauseSelected = 0;
	m_isPaused = false;
	for (auto& item : m_pauseSelection) {
		item.fillamount = 0.0f;
	}
}

void BattleLevel::OnRestart() {
	OnContinue();
	m_character->TakeDamage(1000.0f);
}
