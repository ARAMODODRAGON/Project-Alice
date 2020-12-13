#include "SceneManager.hpp"
#include "Input\detail\SystemEvents.hpp"

namespace ALC {

	uint32 SceneManager::levelToLoad = -1;
	IScene* SceneManager::activeScene = nullptr;
	Game* SceneManager::activeGame = nullptr;
	bool SceneManager::isRunning = false;
	bool SceneManager::shouldQuit = false;

	void SceneManager::StartGame(Game* game_, const std::vector<detail::SceneBinding> bindings) {
		if (bindings.size() == 0) {
			ALC_DEBUG_FATAL_ERROR("No scenes were bound");
		}

		// setup variables
		levelToLoad = -1;
		activeScene = bindings[0]();
		activeGame = game_;
		shouldQuit = false;

		// init
		try {
			activeGame->Init();
			activeScene->Init();
		}
		// catch any errors and quit
		catch (const std::exception& e) {
			ALC_DEBUG_FATAL_ERROR(e.what());
			delete activeScene;
			return;
		}

		while (isRunning && !shouldQuit) {

			if (levelToLoad != -1) {
				if (levelToLoad < bindings.size()) {
					activeScene->Exit();
					delete activeScene;
					activeScene = bindings[levelToLoad]();
					activeScene->Init();
				}
				levelToLoad = -1;				
			}

			detail::SystemEvents::PollEvents();
			activeGame->Step();
			activeScene->Step();

			activeGame->PreDraw();
			activeScene->PreDraw();

			activeGame->Draw();
			activeScene->Draw();

			activeGame->PostDraw();
			activeScene->PostDraw();

		}

		// exit
		try {
			activeGame->Exit();
			activeScene->Exit();
		}
		catch (const std::exception& e) {
			ALC_DEBUG_FATAL_ERROR(e.what());
		}

		delete activeScene, activeScene = nullptr;
	}

}