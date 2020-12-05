#include <iostream>
#include "ALC\SceneManager.hpp"
#include <glew.h>
#include <ALC\Entities\EntityRegistry.hpp>
#include <ALC\Entities\Components.hpp>
#include <ALC\Input\Keyboard.hpp>
#include <glm\gtx\norm.hpp>

class PlayerController final : public ALC::Behavior {
public:

	PlayerController() { }
	~PlayerController() { }

	void Start(ALC::Entity e) override {
		e.AddComponent<ALC::Transform2D>();
	}
	void Update(ALC::Entity e) override { 
		if (e.HasComponent<ALC::Transform2D>()) {
			auto& transform = e.GetComponent<ALC::Transform2D>();

			const auto key_up = ALC::Keyboard::GetKey(ALC::KeyCode::ArrowUp);
			const auto key_down = ALC::Keyboard::GetKey(ALC::KeyCode::ArrowDown);
			const auto key_left = ALC::Keyboard::GetKey(ALC::KeyCode::ArrowLeft);
			const auto key_right = ALC::Keyboard::GetKey(ALC::KeyCode::ArrowRight);
			glm::vec2 input = glm::vec2(key_right.IsHeld() - key_left.IsHeld(), key_up.IsHeld() - key_down.IsHeld());
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
	ALC::EntityRegistry reg;

	BattleScene() : player(nullptr) {
		reg.RegInitFunc([] (ALC::Entity e) {
			e.AddComponent<ALC::Transform2D>();
		});
	}
	~BattleScene() { }
	void Init() override {
		auto e = reg.CreateEntity();
		player = e.AddBehavior<PlayerController>();
	}
	void Exit() override { }
	void Step() override {
		reg.Update();

		reg.LateUpdate();
	}
	void PreDraw() override {

	}
	void Draw() override {
	}
	void PostDraw() override {
		reg.Cleanup();
	}
private:
	PlayerController* player;
};

class AliceGame final : public ALC::Game {
public:
	AliceGame() { }
	~AliceGame() { }

	void Init() override { }
	void Exit() override { }

	void Step() override {

	}

	void PreDraw() override {
		const glm::vec4 color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		GetWindow()->ClearScreen(color);
	}

	void Draw() override {

	}

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