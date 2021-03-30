#include "MainMenuScene.hpp"
#include "../SaveSystem.hpp"

MainMenuScene::MainMenuScene()
	: m_menustate(MenuState::MainSelection)
	, m_nextstate(m_menustate)
	, m_textColor(1.0f, 1.0f, 1.0f, 1.0f)
	, m_greyedTextColor(0.5f, 0.5f, 0.5f, 1.0f)
	, m_highlightColor(0.5f, 0.5f, 0.5f, 1.0f)
	, m_isFading(true)
	, m_fadeIn(true)
	, m_fadeValue(1.0f)
	, m_isTransitioning(false)
	, m_uioffset(0.0f)
	, m_gameProgress(0) {
	m_ui.SetInternalScreenSize(BattleManager::PrefferedResolution());
}

MainMenuScene::~MainMenuScene() { }

void MainMenuScene::Init() {
	using CM = ALC::ContentManager;
	m_gameProgress = SaveSystem::Get(STORY_PROGRESS);

	m_mainMenuTex = CM::LoadTexture(m_storage, "Resources/Textures/UI/Title Screen Sheet.png");

	//auto RobotoBoldItalic = CM::LoadFont(m_storage, "Resources/Fonts/Roboto-BoldItalic.ttf", 50);
	auto RobotoMedium = CM::LoadFont(m_storage, "Resources/Fonts/Roboto-Medium.ttf", 25);

	const ALC::vec2 centerscreen = BattleManager::PrefferedResolution() * 0.5f;
	const ALC::vec2 eighth = BattleManager::PrefferedResolution() / 8.0f;

	m_mainselection.selections.resize(5);
	ALC::vec2 position = centerscreen;
	position.y += eighth.y;
	constexpr float spacing = 10.0f;

	m_mainselection.selections[0].text = "Continue";
	m_mainselection.selections[0].confirmfunc = &MainMenuScene::_ContinueOpt;
	m_mainselection.selections[1].text = "New Game";
	m_mainselection.selections[1].confirmfunc = &MainMenuScene::_NewGameOpt;
	m_mainselection.selections[2].text = "Rewards";
	m_mainselection.selections[2].confirmfunc = &MainMenuScene::_RewardsOpt;
	m_mainselection.selections[3].text = "Settings";
	m_mainselection.selections[3].confirmfunc = &MainMenuScene::_SettingsOpt;
	m_mainselection.selections[4].text = "Quit";
	m_mainselection.selections[4].confirmfunc = &MainMenuScene::_QuitOpt;
	if (m_gameProgress == 0) {
		m_mainselection.selections[0].isSelectable = false;
		m_mainselection.selections[2].isSelectable = false;
		m_mainselection.position = 1;
	}

	for (size_t i = 0; i < m_mainselection.selections.size(); i++) {
		auto& opt = m_mainselection.selections[i];
		opt.font = RobotoMedium;
		opt.dimensions = opt.font.StringDimensions(opt.text);
		opt.position = position;
		position.y += opt.dimensions.y + spacing;
	}

}

void MainMenuScene::Exit() { }

void MainMenuScene::Step(ALC::Timestep ts) {
	using KB = ALC::Keyboard;
	using KC = ALC::KeyCode;
	if (m_menustate == m_nextstate && !m_isFading && !m_isTransitioning) {
		const bool up = KB::GetKey(KC::ArrowUp).Pressed();
		const bool down = KB::GetKey(KC::ArrowDown).Pressed();
		const bool select = KB::GetKey(KC::KeyC).Pressed();
		const bool cancel = KB::GetKey(KC::KeyX).Pressed();


		switch (m_menustate) {
			case MenuState::MainSelection:
				if (cancel) m_mainselection.position = m_mainselection.selections.size() - 1; // last option should be "Quit"
				else		UpdateMenu(m_mainselection, up, down, select, ts);
				break;
			case MenuState::LevelSelect:
				if (cancel) m_nextstate = MenuState::MainSelection;
				else		UpdateMenu(m_levelselect, up, down, select, ts);
				break;
			case MenuState::RewardsMenu:
				if (cancel) m_nextstate = MenuState::MainSelection;
				else		UpdateMenu(m_rewardsmenu, up, down, select, ts);
				break;
			default:
				break;
		}


	}
	// fade 
	else if (m_isFading) {
		if (m_fadeIn) {
			m_fadeValue -= ts * 10.0f;
			if (m_fadeValue < 0.0f) {
				m_isFading = false;
				m_fadeValue = 0.0f;
			}
		} else {
			m_fadeValue += ts * 10.0f;
			if (m_fadeValue > 1.0f) {
				m_isFading = false;
				m_fadeValue = 1.0f;
			}
		}
	}
}

void MainMenuScene::Draw() {
	m_ui.Begin();

	// draw background
	if (m_mainMenuTex) {
		ALC::rect pos(ALC::vec2(0.0f), m_ui.GetInternalScreenSize());
		pos.max.y *= 2.0f;
		// offset
		pos.min.y -= m_uioffset;
		pos.max.y -= m_uioffset;

		ALC::rect target(0.0f, 0.0f, 255.0f, 287.0f);

		// background
		m_ui.DrawQuad(pos, ALC_COLOR_WHITE, target, m_mainMenuTex);

		// title
		target = ALC::rect(256.0f, 0.0f, 415.0f, 62.0f);
		pos = ALC::rect(255.0f, 15.0f - m_uioffset, 1050.0f, 325.0f - m_uioffset);
		m_ui.DrawQuad(pos, ALC_COLOR_WHITE, target, m_mainMenuTex);

		if (m_gameProgress > 0) { // LUPUS
			target = ALC::rect(346.0f, 63.0f, 385.0f, 127.0f);
			pos = ALC::rect(40.0f, 120.0f - m_uioffset, 236.0f, 442.0f - m_uioffset);
			m_ui.DrawQuad(pos, ALC_COLOR_WHITE, target, m_mainMenuTex);
		}
		if (m_gameProgress > 1) { // CASSIOPEIA
			target = ALC::rect(385.0f, 90.0f, 431.0f, 112.0f);
			pos = ALC::rect(20.0f, 582.0f - m_uioffset, 251.0f, 691.0f - m_uioffset);
			m_ui.DrawQuad(pos, ALC_COLOR_WHITE, target, m_mainMenuTex);
		}
		if (m_gameProgress > 2) { // SERPENS
			target = ALC::rect(256.0f, 63.0f, 346.0f, 122.0f);
			pos = ALC::rect(823.0f, 391.0f - m_uioffset, 1275.0f, 687.0f - m_uioffset);
			m_ui.DrawQuad(pos, ALC_COLOR_WHITE, target, m_mainMenuTex);
		}
		if (m_gameProgress > 3) { // DRACO
			target = ALC::rect(416.0f, 0.0f, 482.0f, 89.0f);
			pos = ALC::rect(939.0f, 15.0f - m_uioffset, 1270.0f, 460.0f - m_uioffset);
			m_ui.DrawQuad(pos, ALC_COLOR_WHITE, target, m_mainMenuTex);
		}

	}

	constexpr float margin = 7.0f;

	// draw main menu options
	for (size_t i = 0; i < m_mainselection.selections.size(); i++) {
		auto& opt = m_mainselection.selections[i];
		ALC::vec4& color = (opt.isSelectable ? m_textColor : m_greyedTextColor);
		ALC::vec2 pos = opt.position;
		pos.y -= m_uioffset;

		if (!ALC::NearlyZero(opt.higlightFill)) {
			// draw highlight
			ALC::rect quad;
			// size
			quad.min = -opt.dimensions * 0.5f;
			quad.max = -quad.min;
			// margin
			quad.min.x -= margin;
			quad.min.y -= (margin - 6.0f);
			quad.max.x += margin;
			quad.max.y += margin;
			// widthscale
			quad.min.x *= opt.higlightFill;
			quad.max.x *= opt.higlightFill;
			// position
			quad.min += pos;
			quad.max += pos;

			// draw quad
			m_ui.DrawQuad(quad, m_highlightColor);
		}

		// draw text
		m_ui.DrawText(opt.text, opt.font, pos, color, ALC::HAlign::Center, ALC::VAlign::Middle);
	}

	// draw settings ui

	// draw level select ui

	// draw rewards ui


	// draw fade
	if (m_isFading) {
		ALC::vec4 color = ALC_COLOR_BLACK;
		color.a = m_fadeValue;
		m_ui.DrawQuad(ALC::rect(ALC::vec2(0.0f), m_ui.GetInternalScreenSize()), color);
	}

	m_ui.End();
}

void MainMenuScene::UpdateMenu(MenuList& list, bool up, bool down, bool select, ALC::Timestep ts) {
	auto& selections = list.selections;
	auto& pos = list.position;
	const auto initialpos = pos;

	// moves the cursor up or down to a selectable option
	int move = int(down) - int(up);
	while (move != 0) {
		pos += move;
		if (pos >= selections.size()) {
			pos = initialpos;
			break;
		}
		if (selections[pos].isSelectable) {
			break;
		}
	}

	// update higlight
	for (size_t i = 0; i < selections.size(); i++) {
		auto& opt = selections[i];
		// increase or decrease
		if (i == pos) opt.higlightFill += ts * 10.0f;
		else		  opt.higlightFill -= ts * 10.0f;
		opt.higlightFill = glm::clamp(opt.higlightFill, 0.0f, 1.0f);
	}

	// call function
	if (select && (selections[pos].confirmfunc != nullptr)) {
		(this->*selections[pos].confirmfunc)();
	}
}

void MainMenuScene::_ContinueOpt() { }

void MainMenuScene::_NewGameOpt() {
	SaveSystem::ResetSave();
	ALC::SceneManager::LoadScene(1); // load the first battle
}

void MainMenuScene::_RewardsOpt() { }

void MainMenuScene::_SettingsOpt() { }

void MainMenuScene::_QuitOpt() {
	ALC::SceneManager::Quit();
}
