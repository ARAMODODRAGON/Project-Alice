#include "Game\AliceGame.hpp"
#include "Game\BattleManager.hpp"
#include "Game\Scenes\BattleLevels\Battle1.hpp"
#include "Game\Scenes\BattleLevels\Battle2.hpp"
#include "Game\Scenes\MainMenuScene.hpp"

int main(int argc, char* agrv[]) {

	// prepare the level boundries
	// 0.88f is the ratio of the level bounds in touhou
	// 15.0f is the offset from the top of the screen
	{
		ALC::vec2 prefsize = BattleManager::PrefferedResolution();
		constexpr float ratio = 0.88f;
		constexpr float topoffset = 30.0f;
		const float basewidth = prefsize.x * 0.5f;
		const float baseheight = prefsize.y * 0.5f;
		constexpr float scalar = 1.0f;

		ALC::rect levelBounds;
		float toppos = baseheight - topoffset;
		levelBounds.top = toppos * scalar;
		levelBounds.bottom = -toppos * scalar;
		levelBounds.right = toppos * ratio * scalar;
		levelBounds.left = -toppos * ratio * scalar;

		BattleManager::SetLevelBounds(levelBounds);
		
	}

	ALC::EngineSettings settings;

	settings.window.title = "Project Alice";
	settings.window.size = ALC::uvec2(1280, 720);

	settings.scenes.bindings = {
		ALC_BIND_SCENE(MainMenuScene),
		ALC_BIND_SCENE(Battle1),
		ALC_BIND_SCENE(Battle2)
	};
	settings.scenes.startScene = 0;

	settings.jobsystem.enable = true;
	// min of one job thread, max of 3 job threads
	// its clamping a value of ((cpu_threads / 2) - 1)
	//settings.jobsystem.threadcount = glm::clamp((std::thread::hardware_concurrency() / 2u) - 1u, 1u, 3u);
	settings.jobsystem.threadcount = 15u;

	ALC::SceneManager::Start<AliceGame>(settings);

	return 0;
}