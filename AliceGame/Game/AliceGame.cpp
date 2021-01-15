#include "AliceGame.hpp"

#define DEBUG_FONT_SIZE 20
#define DEBUG_FPS_UPDATE_RATE 17

#ifdef _DEBUG
#define DEBUG_DEFAULT_STATE true
#else
#define DEBUG_DEFAULT_STATE false
#endif

AliceGame::AliceGame() : m_showDebug(DEBUG_DEFAULT_STATE), m_lastFPS(0.0f), m_counter(-1) { }

AliceGame::~AliceGame() { }

void AliceGame::Init() {
	using CM = ALC::ContentManager;
	// load the font and make sure its not tied to a context
	m_debugFont = CM::LoadFont(CM::Default(), "Resources/Fonts/arial.ttf", DEBUG_FONT_SIZE);
}

void AliceGame::Exit() { }

void AliceGame::Step(ALC::Timestep t) {
	++m_counter;

	using ALC::Keyboard;
	using ALC::KeyCode;

	if ((m_counter % DEBUG_FPS_UPDATE_RATE) == 0)
		m_lastFPS = t.GetFPS();

	auto ctrl = Keyboard::GetKey(KeyCode::LeftCtrl);
	auto keyw = Keyboard::GetKey(KeyCode::KeyW);
	if ((ctrl && keyw.Pressed()) || (ctrl.Pressed() && keyw)) {
		m_showDebug = !m_showDebug;
	}
}

void AliceGame::PreDraw() {
	GetWindow()->ClearScreen(ALC_COLOR_BLACK);
}

void AliceGame::Draw() {
	if (m_showDebug) {
		m_ui.Begin();
		m_ui.DrawText("FPS: " + VTOS(m_lastFPS), m_debugFont, ALC::vec2(0.0f, DEBUG_FONT_SIZE));
		m_ui.End();
	}
}

void AliceGame::PostDraw() {
	GetWindow()->SwapBuffers();
	GetTimer()->CheckIfFrameComplete();
}