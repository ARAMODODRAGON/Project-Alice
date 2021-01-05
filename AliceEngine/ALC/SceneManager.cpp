#include "SceneManager.hpp"
#include "Input\detail\SystemEvents.hpp"

namespace ALC {

	uint32 SceneManager::s_levelToLoad = -1;
	IScene* SceneManager::s_activeScene = nullptr;
	Game* SceneManager::s_activeGame = nullptr;
	Window* SceneManager::s_window = nullptr;
	FT_Library SceneManager::s_fontLib = nullptr;
	bool SceneManager::s_isRunning = false;
	bool SceneManager::s_shouldQuit = false;

	void SceneManager::StartGame(Game* game_, const std::vector<SceneBinding> bindings) {
		if (bindings.size() == 0) {
			ALC_DEBUG_FATAL_ERROR("No scenes were bound");
		}

		// setup variables
		s_levelToLoad = -1;
		s_activeScene = bindings[0]();
		s_activeGame = game_;
		s_shouldQuit = false;
		Timer timer;
		s_activeGame->__Initialize(s_window, &timer);
		if (FT_Init_FreeType(&s_fontLib)) {
			ALC_DEBUG_ERROR("Font library was unable to be initialized.");
			return;
		}

		// init
		try {
			s_activeGame->Init();
			s_activeScene->Init();
		}
		// catch any errors and quit
		catch (const std::exception& e) {
			ALC_DEBUG_FATAL_ERROR(e.what());
			delete s_activeScene;
			return;
		}

		while (s_isRunning && !s_shouldQuit) {
			timer.BeginFrame();

			if (s_levelToLoad != -1) {
				if (s_levelToLoad < bindings.size()) {
					s_activeScene->Exit();
					delete s_activeScene;
					s_activeScene = bindings[s_levelToLoad]();
					s_activeScene->Init();
				}
				s_levelToLoad = -1;
			}

			detail::SystemEvents::PollEvents();

			Timestep t = timer.GetTimestep();
			s_activeGame->Step(t);
			s_activeScene->Step(t);

			s_activeGame->PreDraw();
			s_activeScene->PreDraw();

			s_activeGame->Draw();
			s_activeScene->Draw();

			s_activeGame->PostDraw();
			s_activeScene->PostDraw();

		}

		// exit
		try {
			s_activeGame->Exit();
			s_activeScene->Exit();
		} catch (const std::exception& e) {
			ALC_DEBUG_FATAL_ERROR(e.what());
		}

		// cleanup
		FT_Done_FreeType(s_fontLib);
		delete s_activeScene, s_activeScene = nullptr;
	}

}