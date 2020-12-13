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

	void Start(ALC::Entity e) override {
		if (!e.HasComponent<ALC::Transform>())
			e.AddComponent<ALC::Transform>();

		if (!e.HasComponent<ALC::SpriteComponent>())
			e.AddComponent<ALC::SpriteComponent>();
		auto& spr = e.GetComponent<ALC::SpriteComponent>();
		//spr.texture = ALC::ContentManager::LoadTexture("Resources/Textures/Grey Orb Flashing.png");
		spr.bounds = ALC::rect(-8, -8, 8, 8);
		//spr.textureBounds = ALC::rect(ALC::vec2(0.0f), spr.texture.GetSize());

	}
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

			transform.position += input * 1.4f;
		}
	}
	void LateUpdate(ALC::Entity e) override { }
	void OnDestroy(ALC::Entity e) override { }

};

class BattleScene final : public ALC::IScene {
	ALC_NO_COPY(BattleScene);
public:
	ALC::ContentStorage storage;
	ALC::Registry reg;
	ALC::SpriteBatch batch;
	ALC::Camera camera;
	ALC::Texture tex;

	BattleScene() { }
	~BattleScene() { }
	void Init() override {
		// set the context
		ALC::ContentManager::SetContext(storage);

		// setup camera
		camera.SetCameraSize(camera.GetCameraSize() * 0.3f);

		// create our player
		ALC::Entity e = reg.Create();
		e.AddBehavior<PlayerController>();
	}

	void Exit() override { }

	void Step() override {
		reg.UpdateBehaviors();
	}

	void PreDraw() override {
		reg.LateUpdateBehaviors();
	}

	void Draw() override {
		batch.Begin(camera);

		reg.ForeachComponent<ALC::Transform, ALC::SpriteComponent>(
			[this](ALC::Entity e, ALC::Transform& transform, ALC::SpriteComponent& sprite) {
			batch.Draw(transform, sprite);
		});

		batch.End();
	}

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

	const ALC::vector<ALC::SceneBinding> SceneIndex = {
		ALC_BIND_SCENE(BattleScene)
	};

	ALC::SceneManager::Start<AliceGame>("Project Alice", ALC::uvec2(1280, 720), SceneIndex);

	return 0;
}