#include "DemoChara.hpp"

DemoChara::DemoChara()
	: m_state(this), m_timer(0.0f), m_circleshootoffset(0.0f), m_clockwise(true), m_spinspeedmult(1.0f) {
	// set textures
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

	// set the state
	m_state.Add(State_A, &DemoChara::StateA);
	m_state.Add(State_A, &DemoChara::BeginStateA);
	m_state.Add(State_B, &DemoChara::StateB);
	m_state.Add(State_B, &DemoChara::BeginStateB);

}

DemoChara::~DemoChara() { }

void DemoChara::Start(ALC::Entity self) {
	if (!self.HasComponent<ALC::Transform2D>()) self.AddComponent<ALC::Transform2D>();
	if (!self.HasComponent<ALC::Rigidbody2D>()) self.AddComponent<ALC::Rigidbody2D>();
	if (!self.HasComponent<ALC::SpriteComponent>()) self.AddComponent<ALC::SpriteComponent>();

	auto& spr = self.GetComponent<ALC::SpriteComponent>();
	spr.bounds = ALC::rect(-8.0f, -8.0f, 8.0f, 8.0f);

	auto& rb = self.GetComponent<ALC::Rigidbody2D>();
	rb.triggerMask = ALC::Layermask32::ALL;
	rb.triggerMask.SetLayer(0, false);
	auto& circle = rb.SetShape<ALC::CircleShape>();
	circle.radius = 8.0f;
	//spr.texture = ALC::ContentManager::LoadTexture("Resources/Textures/Grey Orb Flashing.png");
	//spr.textureBounds = ALC::rect(ALC::vec2(0.0f), spr.texture.GetSize());

}

void DemoChara::OnDestroy(ALC::Entity self) { }

void DemoChara::Shoot(ALC::Entity self, const float angle, const float speed,
					  const ALC::vec2 position, const ALC::vec4 color) {
	if (!self.HasComponent<ALC::EntityCreatorComponent>())
		self.AddComponent<ALC::EntityCreatorComponent>();
	auto& ecc = self.GetComponent<ALC::EntityCreatorComponent>();

	ecc.Create([angle, speed, position, color](ALC::Entity e) {
		auto& tr = e.AddComponent<ALC::Transform2D>();
		auto& rb = e.AddComponent<ALC::Rigidbody2D>();
		rb.triggerMask.SetLayer(0, true);
		auto& circle = rb.SetShape<ALC::CircleShape>();
		circle.radius = 3.0f;
		auto& spr = e.AddComponent<ALC::SpriteComponent>();
		spr.color = color;
		e.AddComponent<DemoBulletComponent>();
		//sprite.texture = textures[rand() % textures.size()];
		//sprite.textureBounds = ALC::rect(ALC::vec2(0.0f), sprite.texture.GetSize());
		spr.bounds = ALC::rect(-3.0f, -3.0f, 3.0f, 3.0f);
		tr.position = position;
		ALC::vec4 vel = glm::rotateZ(ALC::vec4(0.0f, 1.0f, 0.0f, 1.0f), ALC_TO_RADIANS(angle));
		rb.velocity = ALC::vec2(vel.x, vel.y) * speed;
	});
}

void DemoChara::Update(ALC::Entity self, ALC::Timestep t) {
	// get components
	auto& transform = self.GetComponent<ALC::Transform2D>();
	auto& rigidbody = self.GetComponent<ALC::Rigidbody2D>();
	auto& sprite = self.GetComponent<ALC::SpriteComponent>();

	// get input
	const auto key_up = ALC::Keyboard::GetKey(ALC::KeyCode::ArrowUp);
	const auto key_down = ALC::Keyboard::GetKey(ALC::KeyCode::ArrowDown);
	const auto key_left = ALC::Keyboard::GetKey(ALC::KeyCode::ArrowLeft);
	const auto key_right = ALC::Keyboard::GetKey(ALC::KeyCode::ArrowRight);

	// convert input into velocity
	glm::vec2 input = glm::vec2(key_right.IsHeld() - key_left.IsHeld(), key_up.IsHeld() - key_down.IsHeld());
	if (glm::length2(input) > 0.0f) {
		//ALC_DEBUG_LOG("Moving at " + VTOS(input));
	}
	rigidbody.velocity = input * 60.0f;

	// update the current state
	m_state(self, t);

}

void DemoChara::LateUpdate(ALC::Entity self, ALC::Timestep t) {
	auto& rigidbody = self.GetComponent<ALC::Rigidbody2D>();
	auto& sprite = self.GetComponent<ALC::SpriteComponent>();
	if (rigidbody.GetCollisionCount() > 0)
		sprite.color = ALC_COLOR_RED;
	else
		sprite.color = ALC_COLOR_BLUE;
}

void DemoChara::BeginStateA(const State last, ALC::Entity self, ALC::Timestep t) {
	ALC_DEBUG_LOG("State A Begin");
	//m_circleshootoffset = 0.0f;
	m_timer = 0.0f;
	m_spinspeedmult = 1.0f;
}

void DemoChara::StateA(ALC::Entity self, ALC::Timestep t) {
	ALC::Transform2D& transform = self.GetComponent<ALC::Transform2D>();
	ALC::Rigidbody2D& rigidbody = self.GetComponent<ALC::Rigidbody2D>();
	const auto key_shoot = ALC::Keyboard::GetKey(ALC::KeyCode::KeyC);

	// calculate shoot related variables
	constexpr float shoottime = 0.07f;
	if (m_clockwise)
		m_circleshootoffset += 50.0f * t * m_spinspeedmult;
	else
		m_circleshootoffset -= 50.0f * t * m_spinspeedmult;
	m_spinspeedmult += t * 1.3f;

	// shoot
	if (key_shoot) {
		m_timer += t;
		if (m_timer > shoottime || key_shoot.Pressed()) {
			if (!key_shoot.Pressed())
				m_timer -= shoottime;

			// shoots 'shootcount' bullets in circular formation
			constexpr ALC::uint32 shootcount = 10;
			for (ALC::uint32 i = 0; i < shootcount; i++) {
				// (360.0f / float(shootcount)) the difference in angle if you want to shoot 'shootcount' bullets
				// * float(i) multiplies to get a specific angle
				Shoot(self, (360.0f / float(shootcount)) * float(i) + m_circleshootoffset,
					  80.0f, transform.position, ALC_COLOR_RED);
			}
		}
	}
	// not shooting
	else {
		//m_circleshootoffset = 0.0f;
		m_timer = 0.0f;
		m_spinspeedmult = 1.0f;
		// flip shoot rotation dir
		if (key_shoot.Released()) m_clockwise = !m_clockwise;
	}

	const auto key_switchStates = ALC::Keyboard::GetKey(ALC::KeyCode::KeyX);
	if (key_switchStates.Pressed())
		m_state.ChangeState(State_B);
}

void DemoChara::BeginStateB(const State last, ALC::Entity self, ALC::Timestep t) {
	ALC_DEBUG_LOG("State B Begin");
	//m_circleshootoffset = 0.0f;
	m_timer = 0.0f;
	m_spinspeedmult = 1.0f;
}

void DemoChara::StateB(ALC::Entity self, ALC::Timestep t) {
	ALC::Transform2D& transform = self.GetComponent<ALC::Transform2D>();
	ALC::Rigidbody2D& rigidbody = self.GetComponent<ALC::Rigidbody2D>();
	const auto key_shoot = ALC::Keyboard::GetKey(ALC::KeyCode::KeyC);

	// calculate shoot related variables
	constexpr float shoottime = 0.07f;
	//if (m_clockwise)
	//	m_circleshootoffset += 50.0f * t;
	//else
	//	m_circleshootoffset -= 50.0f * t;
	//m_spinspeedmult += t * 1.3f;

	// shoot
	if (key_shoot) {
		m_timer += t;
		if (m_timer > shoottime || key_shoot.Pressed()) {
			if (!key_shoot.Pressed())
				m_timer -= shoottime;

			// shoots 'shootcount' bullets in circular formation
			constexpr ALC::uint32 shootcount = 10;
			for (ALC::uint32 i = 0; i < shootcount; i++) {
				// (360.0f / float(shootcount)) the difference in angle if you want to shoot 'shootcount' bullets
				// * float(i) multiplies to get a specific angle
				Shoot(self, (360.0f / float(shootcount)) * float(i) + m_circleshootoffset,
					  80.0f, transform.position, ALC_COLOR_BLUE);
			}
		}
	}
	// not shooting
	else {
		//m_circleshootoffset = 0.0f;
		m_timer = 0.0f;
		m_spinspeedmult = 1.0f;
		// flip shoot rotation dir
		if (key_shoot.Released()) m_clockwise = !m_clockwise;
	}

	const auto key_switchStates = ALC::Keyboard::GetKey(ALC::KeyCode::KeyX);
	if (key_switchStates.Pressed())
		m_state.ChangeState(State_A);
}