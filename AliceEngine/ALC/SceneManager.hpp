#ifndef ALC_SCENEMANAGER_HPP
#define ALC_SCENEMANAGER_HPP
#include "General.hpp"
#include "Core\Game.hpp"
#include "Content\ContentManager.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H
#include "Core\Timer.hpp"

namespace ALC {

	class IScene {
	public:
		virtual ~IScene() = 0 { }
		virtual void Init() = 0;
		virtual void Exit() = 0;
		virtual void Step(ALC::Timestep ts) = 0;
		virtual void PreDraw() = 0;
		virtual void Draw() = 0;
		virtual void PostDraw() = 0;
	};

	using SceneBinding = IScene * (*)();
	
	// creates a scenebinding
	#define ALC_BIND_SCENE(SceneType) ::ALC::__BindScene<SceneType>()

	struct EngineSettings final {

		// window settings
		struct {
			string title = "";
			uvec2 size = uvec2(1280u, 720u);
		} window;

		struct {
			vector<SceneBinding> bindings;
			// the scene to start with
			uint32 startScene = 0;
		} scenes;

		// jobsystem settings
		struct {
			bool enable = false;
			// number of threads to allocate in the jobsystem
			// 0 will automatically determine the number of threads
			uint8 threadcount = 0;
		} jobsystem;

	};

	class SceneManager {
		ALC_NON_CONSTRUCTABLE(SceneManager);
	public:

		// starts the engine with the given settings
		// settings will be moved into the scene manager
		template<typename GameTy>
		static void Start(EngineSettings& settings);

		// loads a level based on its index in the scene bindings
		static void LoadScene(const uint32 index) { s_levelToLoad = index; }

		// returns the index of the current scene in the scene bindings
		static uint32 GetActiveSceneBindingIndex() { return s_currentSceneIndex; }

		// checks if the engine is currently running
		static bool IsRunning() { return s_isRunning; }

		// self explanitory
		static void Quit() { s_shouldQuit = true; }

		// returns a pointer to the current scene
		static IScene* GetActiveScene() { return s_activeScene; }

		// returns a pointer to the current game
		static Game* GetActiveGame() { return s_activeGame; }

		// returns a pointer to the window
		static Window* GetWindow() { return s_window; }

	private:
		static void StartGame(Game* game_);
		static EngineSettings s_settings;
		static uint32 s_levelToLoad;
		static IScene* s_activeScene;
		static uint32 s_currentSceneIndex;
		static Game* s_activeGame;
		static Window* s_window;
		static FT_Library s_fontLib;
		static bool s_isRunning;
		static bool s_shouldQuit;
	public:
		static FT_Library __GetFTLibrary() { return s_fontLib; }
	};
	
	template<typename SceneType>
	inline SceneBinding __BindScene() {
		return []()->::ALC::IScene* { return new SceneType(); };
	}

	template<typename GameTy>
	inline void SceneManager::Start(EngineSettings& settings) {
		if (s_isRunning) {
			ALC_DEBUG_ERROR("Game is already running, cannot start another instance");
			return;
		}
		s_settings = std::move(settings);
		s_isRunning = true;
		s_window = new Window(s_settings.window.title, s_settings.window.size);
		Scope<Game> game(new GameTy());
		StartGame(game.get());
		ContentManager::Clear();
		s_isRunning = false;
	}

}

#endif // !ALC_SCENEMANAGER_HPP