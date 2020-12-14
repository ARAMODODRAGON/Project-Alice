#include <iostream>
#include "ALC\SceneManager.hpp"
#include <glew.h>
#include <ALC\Input\Keyboard.hpp>
#include <glm\gtx\norm.hpp>
#include <ALC\Entities\Registry.hpp>
#include <ALC\Rendering\SpriteBatch.hpp>
#include <ALC\Rendering\Camera.hpp>
#include <glm\gtc\random.hpp>
#include <ALC\Physics\Rigidbody2D.hpp>
#include <glm/gtx/vector_angle.hpp>

struct BulletComponent {

	float lifetime = 0.0f;
	float maxlifetime = 2.0f;

};

class PlayerController final : public ALC::Behavior {
	float counter;
public:

	struct ShootData {
		float angle;
		float speed;
		ALC::vec2 position;
	};
	ALC::vector<ShootData> shootdata;

	ALC::array<ALC::Texture, 22> textures;

	PlayerController() : counter(0.0f) {
		textures = {
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
	}
	~PlayerController() { }

	void Start(ALC::Entity e) override {
		if (!e.HasComponent<ALC::Transform2D>())
			e.AddComponent<ALC::Transform2D>();
		if (!e.HasComponent<ALC::Rigidbody2D>())
			e.AddComponent<ALC::Rigidbody2D>();

		if (!e.HasComponent<ALC::SpriteComponent>())
			e.AddComponent<ALC::SpriteComponent>();
		auto& spr = e.GetComponent<ALC::SpriteComponent>();
		//spr.texture = ALC::ContentManager::LoadTexture("Resources/Textures/Grey Orb Flashing.png");
		spr.bounds = ALC::rect(-8, -8, 8, 8);
		//spr.textureBounds = ALC::rect(ALC::vec2(0.0f), spr.texture.GetSize());

	}
	void Shoot(const float angle, const float speed, const ALC::vec2 position) {
		if (ALC::NearlyZero(position, 1.0f)) {
			ALC_DEBUG_LOG("DOOP");
		}
		ShootData dat;
		dat.angle = angle;
		dat.speed = speed;
		dat.position = position;
		shootdata.push_back(dat);
	}
	void Update(ALC::Entity e) override {
		if (e.HasComponent<ALC::Transform2D, ALC::Rigidbody2D>()) {
			ALC::Transform2D& transform = e.GetComponent<ALC::Transform2D>();
			ALC::Rigidbody2D& rigidbody = e.GetComponent<ALC::Rigidbody2D>();

			const auto key_up = ALC::Keyboard::GetKey(ALC::KeyCode::ArrowUp);
			const auto key_down = ALC::Keyboard::GetKey(ALC::KeyCode::ArrowDown);
			const auto key_left = ALC::Keyboard::GetKey(ALC::KeyCode::ArrowLeft);
			const auto key_right = ALC::Keyboard::GetKey(ALC::KeyCode::ArrowRight);
			glm::vec2 input = glm::vec2(key_right.IsHeld() - key_left.IsHeld(), key_up.IsHeld() - key_down.IsHeld());
			if (glm::length2(input) > 0.0f) {
				//ALC_DEBUG_LOG("Moving at " + VTOS(input));
			}

			rigidbody.velocity = input * 60.0f;

			const auto key_shoot = ALC::Keyboard::GetKey(ALC::KeyCode::KeyC);
			if (key_shoot) {
				const float delta = ALC::SceneManager::GetActiveGame()->GetTimer()->GetDelta();
				counter += delta;
				if (counter > 1.0f || key_shoot.Pressed()) {
					if (!key_shoot.Pressed())
						counter -= 1.0f;
					for (ALC::uint32 i = 0; i < 9; i++) {
						Shoot((360.0f / 9.0f) * float(i), 80.0f, transform.position);
					}
				}
			} else {
				counter = 0.0f;
			}
		}
	}
	void LateUpdate(ALC::Entity e) override { }
	void OnDestroy(ALC::Entity e) override { }

};

#define ENTITY_COUNT 10000

class BattleScene final : public ALC::IScene {
	ALC_NO_COPY(BattleScene);
public:
	ALC::ContentStorage storage;
	ALC::Registry reg;
	ALC::SpriteBatch batch;
	ALC::Camera camera;
	ALC::Texture tex;
	PlayerController* player;

	BattleScene() { }
	~BattleScene() { }
	void Init() override {
		// set the context
		ALC::ContentManager::SetContext(storage);

		// setup camera
		camera.SetCameraSize(camera.GetCameraSize() * 0.3f);
		ALC::vec2 camsize = camera.GetCameraSize();

		// create our player
		player = reg.Create().AddBehavior<PlayerController>();

		//ALC::Texture textures[] = {
		//	ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/1B Ninja.png"),
		//	ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/AAEEEIOU.png"),
		//	ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/AmazedFace.png"),
		//	ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/Ayyad.jpg"),
		//	ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/ayyad.png"),
		//	ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/Ayyyad.jpg"),
		//	ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/Dwane_Face.png"),
		//	ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/HACK.png"),
		//	ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/IMG_2312.JPG"),
		//	ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/King_Sean.png"),
		//	ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/mohd.png"),
		//	ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/MrV.png"),
		//	ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/NIGHT.png"),
		//	ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/P.E.E.P.png"),
		//	ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/PizzaTime.png"),
		//	ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/Random.png"),
		//	ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/Random_Purple.png"),
		//	ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/Random_Yellow.png"),
		//	ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/Scott.png"),
		//	ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/SeanToxic.png"),
		//	ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/Slick Shades Devil.png"),
		//	ALC::ContentManager::LoadTexture("Resources/Textures/BatchDemo/unknown.png")
		//};
		//constexpr size_t texture_count = sizeof(textures) / sizeof(ALC::Texture);
		//
		//srand(unsigned int(std::chrono::steady_clock::now().time_since_epoch().count()));
		//// create 500 objects with random positions
		//for (size_t i = 0; i < ENTITY_COUNT; i++) {
		//	ALC::Entity e = reg.Create();
		//	auto& transform = e.AddComponent<ALC::Transform2D>();
		//	auto& sprite = e.AddComponent<ALC::SpriteComponent>();
		//	auto& body = e.AddComponent<ALC::Rigidbody2D>();
		//	body.velocity = glm::circularRand(1.0f) * (0.2f + 0.8f * (float(rand()) / float(RAND_MAX))) * 60.0f;
		//
		//	sprite.texture = textures[rand() % texture_count];
		//	sprite.textureBounds = ALC::rect(ALC::vec2(0.0f), sprite.texture.GetSize());
		//	sprite.bounds = ALC::rect(-8, -8, 8, 8);
		//
		//	transform.position = glm::linearRand(-camsize * 0.5f, camsize * 0.5f);
		//	switch (rand() % 4) {
		//		case 0: sprite.color = ALC_COLOR_RED; break;
		//		case 1: sprite.color = ALC_COLOR_BLUE; break;
		//		case 2: sprite.color = ALC_COLOR_GREEN; break;
		//		case 3: sprite.color = ALC_COLOR_WHITE; break;
		//		default: break;
		//	}
		//	sprite.color = glm::mix(sprite.color, ALC_COLOR_WHITE, (float(rand()) / float(RAND_MAX)));
		//	sprite.color.a = 0.7f * (float(rand()) / float(RAND_MAX));
		//
		//}
		//ALC_DEBUG_LOG(VTOS(ENTITY_COUNT) + " entities loaded");
	}

	void Exit() override { }

	void Shoot(PlayerController::ShootData& dat) {
		ALC::Entity e = reg.Create();
		auto& transform = e.AddComponent<ALC::Transform2D>();
		auto& rigidbody = e.AddComponent<ALC::Rigidbody2D>();
		e.AddComponent<BulletComponent>();
		auto& sprite = e.AddComponent<ALC::SpriteComponent>();
		//sprite.texture = textures[rand() % textures.size()];
		//sprite.textureBounds = ALC::rect(ALC::vec2(0.0f), sprite.texture.GetSize());
		sprite.bounds = ALC::rect(-3, -3, 3, 3);
		transform.position = dat.position;
		ALC::vec4 vel = glm::rotateZ(ALC::vec4(0.0f, 1.0f, 0.0f, 1.0f), ALC_TO_RADIANS(dat.angle));
		rigidbody.velocity = ALC::vec2(vel.x, vel.y) * dat.speed;
	}

	void Step() override {
		reg.UpdateBehaviors();

		ALC::vec2 bounds = camera.GetCameraSize() * 0.5f;
		const float delta = ALC::SceneManager::GetActiveGame()->GetTimer()->GetDelta();
		reg.ForeachComponent<ALC::Transform2D, ALC::Rigidbody2D>(
			[delta, bounds](ALC::Entity e, ALC::Transform2D& t, ALC::Rigidbody2D& r) {
			t.position += r.velocity * delta;

			//if (t.position.x < -bounds.x) {
			//	r.velocity.x = -r.velocity.x;
			//	t.position.x += 0.1f;
			//}
			//
			//if (t.position.x > bounds.x) {
			//	r.velocity.x = -r.velocity.x;
			//	t.position.x -= 0.1f;
			//}
			//
			//if (t.position.y < -bounds.y) {
			//	r.velocity.y = -r.velocity.y;
			//	t.position.y += 0.1f;
			//}
			//
			//if (t.position.y > bounds.y) {
			//	r.velocity.y = -r.velocity.y;
			//	t.position.y -= 0.1f;
			//}
		});

		if (player && player->shootdata.size()) {
			for (size_t i = 0; i < player->shootdata.size(); i++) {
				Shoot(player->shootdata[i]);
			}
			player->shootdata.clear();
		}

		//reg.ForeachComponent<BulletComponent>(
		//	[delta, this](ALC::Entity e, BulletComponent& bul) {
		//	bul.lifetime += delta;
		//	if (bul.lifetime > bul.maxlifetime) {
		//		reg.DestroyEntity(e);
		//		//ALC_DEBUG_LOG("Destroy!");
		//	}
		//});
	}

	void PreDraw() override {
		reg.LateUpdateBehaviors();
	}

	void Draw() override {
		batch.Begin(camera);

		reg.ForeachComponent<ALC::Transform2D, ALC::SpriteComponent>(
			[this](ALC::Entity e, ALC::Transform2D& transform, ALC::SpriteComponent& sprite) {
			batch.Draw(transform, sprite);
		});

		batch.End();
	}

	void PostDraw() override {
		reg.Cleanup();
	}
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
		static ALC::uint32 counter = 0;
		if (!(++counter % 60))
			ALC_DEBUG_LOG("FPS: " + VTOS(GetTimer()->GetFPS()));
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