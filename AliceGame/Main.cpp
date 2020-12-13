#include <iostream>
#include "ALC\SceneManager.hpp"
#include <glew.h>
#include <ALC\Input\Keyboard.hpp>
#include <glm\gtx\norm.hpp>
#include <ALC\Entities\Registry.hpp>
#include <ALC\Rendering\SpriteBatch.hpp>
#include <ALC\Rendering\Camera.hpp>
#include <glm\gtc\random.hpp>


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
				//ALC_DEBUG_LOG("Moving at " + VTOS(input));
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
		ALC::vec3 camsize = ALC::vec3(camera.GetCameraSize(), 0.0f);

		// create our player
		reg.Create().AddBehavior<PlayerController>();

		ALC::Texture textures[] = {
			ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/1B Ninja.png"),
			ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/AAEEEIOU.png"),
			ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/AmazedFace.png"),
			ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/Ayyad.jpg"),
			ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/ayyad.png"),
			ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/Ayyyad.jpg"),
			ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/Dwane_Face.png"),
			ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/HACK.png"),
			ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/IMG_2312.JPG"),
			ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/King_Sean.png"),
			ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/mohd.png"),
			ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/MrV.png"),
			ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/NIGHT.png"),
			ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/P.E.E.P.png"),
			ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/PizzaTime.png"),
			ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/Random.png"),
			ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/Random_Purple.png"),
			ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/Random_Yellow.png"),
			ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/Scott.png"),
			ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/SeanToxic.png"),
			ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/Slick Shades Devil.png"),
			ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/unknown.png")
		};
		constexpr size_t texture_count = sizeof(textures) / sizeof(ALC::Texture);

		srand(unsigned int(std::chrono::steady_clock::now().time_since_epoch().count()));
		// create 500 objects with random positions
		for (size_t i = 0; i < 4000; i++) {
			ALC::Entity e = reg.Create();
			auto& transform = e.AddComponent<ALC::Transform>();
			auto& sprite = e.AddComponent<ALC::SpriteComponent>();

			sprite.texture = textures[rand() % texture_count];
			sprite.textureBounds = ALC::rect(ALC::vec2(0.0f), sprite.texture.GetSize());

			transform.position = glm::linearRand(-camsize * 0.5f, camsize * 0.5f);
			sprite.bounds = ALC::rect(-8, -8, 8, 8);
			switch (rand() % 4) {
				case 0: sprite.color = ALC_COLOR_RED; break;
				case 1: sprite.color = ALC_COLOR_BLUE; break;
				case 2: sprite.color = ALC_COLOR_GREEN; break;
				case 3: sprite.color = ALC_COLOR_WHITE; break;
				default: break;
			}
			sprite.color = glm::mix(sprite.color, ALC_COLOR_WHITE, (float(rand()) / float(RAND_MAX)));
			sprite.color.a = 0.1f + 0.9f * (float(rand()) / float(RAND_MAX));

		}

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

	void Init() override {
		GetTimer()->SetTargetFPS(60);
	}
	void Exit() override { }

	void Step() override {
		//ALC_DEBUG_LOG("FPS: " + VTOS(GetTimer()->GetFPS()));
	}

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