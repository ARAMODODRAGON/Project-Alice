#include "DemoChara.hpp"

void DemoChara::Start(ALC::Entity self) {
	if (!self.HasComponent<ALC::Transform2D>()) self.AddComponent<ALC::Transform2D>();
	if (!self.HasComponent<ALC::Rigidbody2D>()) self.AddComponent<ALC::Rigidbody2D>();
	if (!self.HasComponent<ALC::SpriteComponent>()) self.AddComponent<ALC::SpriteComponent>();

	auto& spr = self.GetComponent<ALC::SpriteComponent>();
	//spr.texture = ALC::ContentManager::LoadTexture("Resources/Textures/Grey Orb Flashing.png");
	spr.bounds = ALC::rect(-8, -8, 8, 8);
	//spr.textureBounds = ALC::rect(ALC::vec2(0.0f), spr.texture.GetSize());

}

void DemoChara::OnDestroy(ALC::Entity self) { }

void DemoChara::Shoot(ALC::Entity self, const float angle, const float speed, const ALC::vec2 position) {
	if (!self.HasComponent<ALC::EntityCreatorComponent>())
		self.AddComponent<ALC::EntityCreatorComponent>();
	auto& ecc = self.GetComponent<ALC::EntityCreatorComponent>();

	ecc.Create([angle, speed, position](ALC::Entity e) {
		auto& transform = e.AddComponent<ALC::Transform2D>();
		auto& rigidbody = e.AddComponent<ALC::Rigidbody2D>();
		e.AddComponent<BulletComponent>();
		auto& sprite = e.AddComponent<ALC::SpriteComponent>();
		//sprite.texture = textures[rand() % textures.size()];
		//sprite.textureBounds = ALC::rect(ALC::vec2(0.0f), sprite.texture.GetSize());
		sprite.bounds = ALC::rect(-3, -3, 3, 3);
		transform.position = position;
		ALC::vec4 vel = glm::rotateZ(ALC::vec4(0.0f, 1.0f, 0.0f, 1.0f), ALC_TO_RADIANS(angle));
		rigidbody.velocity = ALC::vec2(vel.x, vel.y) * speed;
	});
}

void DemoChara::Update(ALC::Entity self, ALC::Timestep t) {
	if (self.HasComponent<ALC::Transform2D, ALC::Rigidbody2D>()) {
		ALC::Transform2D& transform = self.GetComponent<ALC::Transform2D>();
		ALC::Rigidbody2D& rigidbody = self.GetComponent<ALC::Rigidbody2D>();

		const auto key_up = ALC::Keyboard::GetKey(ALC::KeyCode::ArrowUp);
		const auto key_down = ALC::Keyboard::GetKey(ALC::KeyCode::ArrowDown);
		const auto key_left = ALC::Keyboard::GetKey(ALC::KeyCode::ArrowLeft);
		const auto key_right = ALC::Keyboard::GetKey(ALC::KeyCode::ArrowRight);
		glm::vec2 input = glm::vec2(key_right.IsHeld() - key_left.IsHeld(), key_up.IsHeld() - key_down.IsHeld());
		if (glm::length2(input) > 0.0f) {
			//ALC_DEBUG_LOG("Moving at " + VTOS(input));
		}

		rigidbody.velocity = input * 60.0f;

		constexpr float shoottime = 0.07f;
		const auto key_shoot = ALC::Keyboard::GetKey(ALC::KeyCode::KeyC);
		if (clockwise)
			circleshootoffset += 50.0f * t * spinspeedmult;
		else
			circleshootoffset -= 50.0f * t * spinspeedmult;
		spinspeedmult += t * 1.3f;

		if (key_shoot) {
			timer += t;
			if (timer > shoottime || key_shoot.Pressed()) {
				if (!key_shoot.Pressed())
					timer -= shoottime;
				constexpr ALC::uint32 shootcount = 9;
				for (ALC::uint32 i = 0; i < shootcount; i++) {
					// (360.0f / float(shootcount)) the difference in angle if you want to shoot 'shootcount' bullets
					// * float(i) multiplies to get a specific angle
					Shoot(self, (360.0f / float(shootcount)) * float(i) + circleshootoffset, 80.0f, transform.position);
				}
			}
		} else {
			if (key_shoot.Released()) clockwise = !clockwise;
			circleshootoffset = 0.0f;
			timer = 0.0f;
			spinspeedmult = 1.0f;
		}
	}
}

void DemoChara::LateUpdate(ALC::Entity self, ALC::Timestep t) { }
