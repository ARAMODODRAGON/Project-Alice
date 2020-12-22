#include "Game\AliceGame.hpp"
#include "Game\Scenes\DemoLevel.hpp"

int main(int argc, char* agrv[]) {



	const ALC::vector<ALC::SceneBinding> SceneIndex = {
		ALC_BIND_SCENE(DemoLevel)
	};
	
	ALC::SceneManager::Start<AliceGame>("Project Alice", ALC::uvec2(1280, 720), SceneIndex);

	return 0;
}