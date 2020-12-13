#include <iostream>
#include "ALC\SceneManager.hpp"
#include <glew.h>
#include <ALC\Input\Keyboard.hpp>
#include <glm\gtx\norm.hpp>
#include <ALC\Entities\Registry.hpp>
#include <ALC\Rendering\SpriteBatch.hpp>
#include <ALC\Rendering\Camera.hpp>

class PlayerController final : public ALC::Behavior {
public:

	PlayerController() { }
	~PlayerController() { }

	void Start(ALC::Entity e) override { }
	void Update(ALC::Entity e) override {
		if (e.HasComponent<ALC::Transform>()) {
			auto& transform = e.GetComponent<ALC::Transform>();

			const auto key_up = ALC::Keyboard::GetKey(ALC::KeyCode::ArrowUp);
			const auto key_down = ALC::Keyboard::GetKey(ALC::KeyCode::ArrowDown);
			const auto key_left = ALC::Keyboard::GetKey(ALC::KeyCode::ArrowLeft);
			const auto key_right = ALC::Keyboard::GetKey(ALC::KeyCode::ArrowRight);
			glm::vec3 input = glm::vec3(key_right.IsHeld() - key_left.IsHeld(), key_up.IsHeld() - key_down.IsHeld(), 0.0f);
			if (glm::length2(input) > 0.0f) {
				ALC_DEBUG_LOG("Moving at " + VTOS(input));
			}

			transform.position += input;
		}
	}
	void LateUpdate(ALC::Entity e) override { }
	void OnDestroy(ALC::Entity e) override { }

};

class BattleScene final : public ALC::IScene {
public:
	ALC::Registry reg;
	ALC::SpriteBatch spritebatch;
	ALC::Camera camera;

	BattleScene() { }
	~BattleScene() { }
	void Init() override {
		ALC::Entity e = reg.Create();
		e.AddBehavior<PlayerController>();
	}

	void Exit() override { }
	void Step() override { }
	void PreDraw() override { }
	void Draw() override { }
	void PostDraw() override { }
};

class AliceGame final : public ALC::Game {
public:
	AliceGame() { }
	~AliceGame() { }

	void Init() override { }
	void Exit() override { }

	void Step() override { }

	void PreDraw() override {
		GetWindow()->ClearScreen(ALC_COLOR_BLACK);
	}

	void Draw() override { }

	void PostDraw() override {
		GetTimer()->WaitForEndOfFrame();
		GetWindow()->SwapBuffers();
	}

};

int main(int argc, char* agrv[]) {

	const auto SceneIndex = {
		ALC_BIND_SCENE(BattleScene)
	};

	ALC::SceneManager::Start<AliceGame>("Project Alice", glm::uvec2(1280, 720), SceneIndex);

	return 0;
}