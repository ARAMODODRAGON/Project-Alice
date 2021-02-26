#ifndef ALICE_MAINMENUSCENE_HPP
#define ALICE_MAINMENUSCENE_HPP
#include <ALC\SceneManager.hpp>
#include <ALC\Rendering.hpp>
#include <ALC\Input.hpp>
#include "../BattleManager.hpp"

class MainMenuScene final : ALC::IScene {
public:

	MainMenuScene();
	~MainMenuScene();

	void Init() override;
	void Exit() override;
	void Step(ALC::Timestep ts) override;
	void Draw() override;

private:

	ALC::ContentStorage m_storage;
	ALC::UIBatch m_ui;
	ALC::Texture m_mainMenuTex;

	struct UISelection final {
		ALC::vec2 position;
		ALC::vec2 dimensions;
		ALC::string text;
		ALC::Font font;
		float higlightFill = 0.0f;
		bool isSelectable = true;
		ALC::function<void()> func;
	};

	struct MenuList final {
		ALC::vector<UISelection> selections;
		ALC::uint32 position = 0;
	};

	enum class MenuState : ALC::uint8 {
		MainSelection,
		LevelSelect,
		RewardsMenu
	};

	void UpdateMenu(MenuList& list, bool up, bool down, bool select);

	MenuState m_menustate;
	MenuState m_nextstate;

	MenuList m_mainselection;
	MenuList m_levelselect;
	MenuList m_rewardsmenu;

	ALC::vec4 m_textColor;
	ALC::vec4 m_greyedTextColor;
	ALC::vec4 m_highlightColor;

	bool m_isFading;
	bool m_fadeIn;
	float m_fadeValue;

	ALC::int32 m_isTransitioning;
	float m_uioffset;

};

#endif // !ALICE_MAINMENUSCENE_HPP