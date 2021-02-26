#include "MainMenuScene.hpp"

MainMenuScene::MainMenuScene()
	: m_menustate(MenuState::MainSelection)
	, m_nextstate(m_menustate)
	, m_textColor(1.0f, 1.0f, 1.0f, 1.0f)
	, m_greyedTextColor(0.5f, 0.5f, 0.5f, 1.0f)
	, m_highlightColor(0.0f, 0.0f, 1.0f, 1.0f)
	, m_isFading(true)
	, m_fadeIn(false)
	, m_fadeValue(1.0f)
	, m_isTransitioning(false)
	, m_uioffset(0.0f) {
	m_ui.SetInternalScreenSize(BattleManager::PrefferedResolution());
}

MainMenuScene::~MainMenuScene() { }

void MainMenuScene::Init() {
	using CM = ALC::ContentManager;

	//auto RobotoBoldItalic = CM::LoadFont(m_storage, "Resources/Fonts/Roboto-BoldItalic.ttf", 50);
	auto RobotoMedium = CM::LoadFont(m_storage, "Resources/Fonts/Roboto-Medium.ttf", 20);

	m_mainselection.selections.resize(5);
	for (size_t i = 0; i < m_mainselection.selections.size(); i++)
		m_mainselection.selections[i].font = RobotoMedium;
	ALC::vec2 position = BattleManager::PrefferedResolution() * 0.5f;
	constexpr float spacing = 10.0f;

	{
		auto& continueOpt = m_mainselection.selections[0];
		continueOpt.text = "Continue";
		continueOpt.dimensions = continueOpt.font.StringDimensions(continueOpt.text);
		continueOpt.position = position;
		position.y += continueOpt.dimensions.y + spacing;
		//continueOpt.isSelectable// TODO

		auto& newGameOpt = m_mainselection.selections[1];
		newGameOpt.text = "New Game";
		continueOpt.dimensions = continueOpt.font.StringDimensions(continueOpt.text);

		auto& rewardsOpt = m_mainselection.selections[2];
		rewardsOpt.text = "Rewards";
		continueOpt.dimensions = continueOpt.font.StringDimensions(continueOpt.text);
		//rewardsOpt.isSelectable// TODO

		auto& settingsOpt = m_mainselection.selections[3];
		settingsOpt.text = "Settings";
		settingsOpt.dimensions = settingsOpt.font.StringDimensions(settingsOpt.text);

		auto& quitOpt = m_mainselection.selections[4];
		quitOpt.text = "Quit";
		quitOpt.dimensions = quitOpt.font.StringDimensions(quitOpt.text);

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
				else		UpdateMenu(m_mainselection, up, down, select);
				break;
			case MenuState::LevelSelect:
				if (cancel) m_nextstate = MenuState::MainSelection;
				else		UpdateMenu(m_levelselect, up, down, select);
				break;
			case MenuState::RewardsMenu:
				if (cancel) m_nextstate = MenuState::MainSelection;
				else		UpdateMenu(m_rewardsmenu, up, down, select);
				break;
			default:
				break;
		}


	}
}

void MainMenuScene::Draw() {
	m_ui.Begin();

	m_ui.End();
}

void MainMenuScene::UpdateMenu(MenuList& list, bool up, bool down, bool select) {
	auto& selections = list.selections;
	auto& pos = list.position;

	// moves the cursor up or down to a selectable option
	int move = int(down) - int(up);
	while (move != 0) {
		pos += move;
		pos %= selections.size();
		if (selections[pos].isSelectable)
			move = 0;
	}

	if (select && (selections[pos].func != nullptr)) {
		selections[pos].func();
	}
}
