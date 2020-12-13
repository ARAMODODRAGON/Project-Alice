#ifndef ALC_SCENEMANAGER_HPP
#define ALC_SCENEMANAGER_HPP
#include "General.hpp"
#include "Core\Game.hpp"
#include "Content\ContentManager.hpp"

namespace ALC {

	class IScene {
	public:
		virtual ~IScene() = 0 { }
		virtual void Init() = 0;
		virtual void Exit() = 0;
		virtual void Step() = 0;
		virtual void PreDraw() = 0;
		virtual void Draw() = 0;
		virtual void PostDraw() = 0;
	};

	namespace detail {
		using SceneBinding = IScene * (*)();
	}
	#define ALC_BIND_SCENE(SceneType) (::ALC::detail::SceneBinding)[]()->::ALC::IScene* { return new SceneType(); }

	class SceneManager {
		ALC_NON_CONSTRUCTABLE(SceneManager);
	public:

		template<typename GameTy, typename Bindings>
		static void Start(const string& title_, const uvec2& windowSize_, Bindings&& bindings);

		static void LoadLevel(const uint32 index) { levelToLoad = index; }
		static bool IsRunning() { return isRunning; }
		static void Quit() { shouldQuit = true; }
		static IScene* GetActiveScene() { return activeScene; }
		static Game* GetActiveGame() { return activeGame; }

	private:

		static void StartGame(Game* game_, const std::vector<detail::SceneBinding> bindings);
		static uint32 levelToLoad;
		static IScene* activeScene;
		static Game* activeGame;
		static bool isRunning;
		static bool shouldQuit;

	};

	template<typename GameTy, typename Bindings>
	inline void ALC::SceneManager::Start(const string& title_, const uvec2& windowSize_, Bindings&& bindings) {
		if (isRunning) {
			ALC_DEBUG_ERROR("Game is already running, cannot start another instance");
			return;
		}
		isRunning = true;
		Scope<Game> game(new GameTy());
		game->__Initialize(title_, windowSize_);
		StartGame(game.get(), bindings);
		ContentManager::Clear();
		isRunning = false;
	}

}

#endif // !ALC_SCENEMANAGER_HPP