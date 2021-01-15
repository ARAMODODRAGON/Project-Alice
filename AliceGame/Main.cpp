#include "Game\AliceGame.hpp"
#include "Game\BattleManager.hpp"
#include "Game\Scenes\BattleLevels\DemoBTL.hpp"

int main(int argc, char* agrv[]) {

	// prepare the level boundries
	// based on if the window is 712x400
	// 0.88f is the ratio of the level bounds in touhou
	// 15.0f is the offset from the top of the screen
	{
		constexpr float ratio = 0.88f;
		constexpr float topoffset = 15.0f;
		constexpr float basewidth = 712.0f * 0.5f;
		constexpr float baseheight = 400.0f * 0.5f;
		constexpr float scalar = 0.3f;

		ALC::rect levelBounds;
		float toppos = baseheight - topoffset;
		levelBounds.top = toppos * scalar;
		levelBounds.bottom = -toppos * scalar;
		levelBounds.right = toppos * ratio * scalar;
		levelBounds.left = -toppos * ratio * scalar;

		BattleManager::SetLevelBounds(levelBounds);
	}

	const ALC::vector<ALC::SceneBinding> SceneIndex = {
		ALC_BIND_SCENE(DemoBTL)
	};

	ALC::SceneManager::Start<AliceGame>("Project Alice", ALC::uvec2(1280, 720), SceneIndex);

	return 0;
}