#ifndef ALICE_SCENES_BATTLELEVEL_HPP
#define ALICE_SCENES_BATTLELEVEL_HPP
#include <ALC\SceneManager.hpp>
#include <ALC\Entities.hpp>
#include "..\Bullets.hpp"
#include "..\Bullets\BulletPhysicsSystem.hpp"
#include <ALC\Rendering.hpp>
#include <ALC\Input.hpp>
#include "../BattleManager.hpp"
#include "../DialogueHandler.hpp"
#include "../JobsECSAdapter.hpp"

class BattleLevel : public ALC::IScene {
public:

	BattleLevel();
	virtual ~BattleLevel() = 0;

	ALC::Registry& GetReg() { return m_reg; }
	ALC::ContentStorage& GetStorage() { return m_storage; }
	ALC::Camera& GetCamera() { return m_camera; }
	ALC::SpriteBatch& GetSpr() { return m_batch; }
	ALC::UIBatch& GetUI() { return m_ui; }
	ALC::EntityCreationHandler& GetECH() { return m_ech; }

	void SetUIOverlay(ALC::Texture UIOverlay) { m_UIOverlay = UIOverlay; }
	void SetUIElements(ALC::Texture UIElements) { m_UIElements = UIElements; }

	// return the levelbounds in screen coordinates
	// uses the BattleManager::PreferredResolution()
	ALC::rect GetScreenLevelBounds() const;

	// return the levelbounds in screen coordinates
	ALC::rect GetScreenLevelBounds(const ALC::vec2& screensize) const;

	float GetTimescale() const { return m_timescale; }
	void SetTimescale(const float timescale) { m_timescale = timescale; }

	Character* GetCharacter() const { return m_character; }

	JobsECSAdapter& GetJobs() { return m_jobsadp; }

protected:

	virtual void DrawBackground(ALC::UIBatch& ui) = 0;
	virtual void GameStep(ALC::Timestep t) = 0;
	virtual void Init();
	virtual void Exit();
	virtual void Draw();
	virtual void PostDraw() { }
	virtual void PreDraw() { }
	void Step(ALC::Timestep t) override;

private:

	ALC::Texture m_UIOverlay;
	ALC::Texture m_UIElements;
	ALC::ContentStorage m_storage;
	ALC::Registry m_reg;
	ALC::EntityCreationHandler m_ech;
	ALC::SpriteBatch m_batch;
	ALC::UIBatch m_ui;
	ALC::Camera m_camera;
	BulletPhysicsSystem m_bPhysics;
	JobsECSAdapter m_jobsadp;
	Character* m_character;
	float m_timescale;

	// debug stuff
	bool m_debug;
	float m_lastFPS;
	double m_delta;
	ALC::uint32 m_counter;
	ALC::Font m_debugFont;

	// ui stuff

	//enum class UIState : ALC::uint8 {
	//	PauseMenu,
	//	SettingsMenu
	//};

	struct UIItem final {
		float fillamount = 0.0f;
		ALC::vec2 dimensions;
		ALC::vec2 position;
		ALC::string text;
		ALC::Font font;
		void(BattleLevel::*func)() = nullptr;
	};

	float m_itemsOffset;
	float m_itemFillspeed;
	UIItem m_itemPaused;
	ALC::array<UIItem, 2> m_pauseSelection;
	ALC::int32 m_pauseSelected;

	void OnContinue();
	void OnRestart();

	bool m_shouldRestart;
	bool m_playerWasDead;
	bool m_isPaused;
	bool m_isFading;
	bool m_shouldFadeIn;
	float m_fadeTransition;
	float m_fadeMaxTransitionTime;
	float m_pauseTransition;
	float m_pauseMaxTransitionTime;
	float m_reloadDelay;

	bool m_showDialogue;
	float m_dialogueTransition;
	float m_dialogueMaxTransitionTime;
	
	ALC::string m_dialogueVisibleText;
	ALC::uint32 m_curCharacter;
	float m_dialogueSpeed;

	ALC::Font m_lifetimeFont;

	// TESTING
	ALC::string multiLineText;
	ALC::vec2 stringDimensions;

};

#endif // !ALICE_SCENES_BATTLELEVEL_HPP