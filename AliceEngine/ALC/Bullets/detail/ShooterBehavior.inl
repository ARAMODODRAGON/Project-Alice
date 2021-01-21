//#include "..\ShooterBehavior.hpp"


inline void ALC::ShooterBehavior::SetDefaultVelocity(const vec2& defaultVelocity) {
	m_defaultVelocity = defaultVelocity;
}

inline void ALC::ShooterBehavior::SetDefaultPosition(const vec2& defaultPosition) {
	m_defaultPosition = defaultPosition;
}

inline void ALC::ShooterBehavior::SetDefaultCollisionmask(const Layermask32& defaultCollisionmask) { 
	m_defaultCollisionmask = defaultCollisionmask;
}

template<typename... Components>
inline void ALC::ShooterBehavior::SetBulletTypes() {
	if constexpr (sizeof...(Components) > 0) {
		m_attachcomponents = [](Entity e)->void {
			// make tuple used to garuntee that we can create multiple components
			auto t = std::make_tuple(e.AddComponent<Components>()...);
		};
	} else {
		m_attachcomponents = nullptr;
	}
}

inline void ALC::ShooterBehavior::Shoot(Entity self, const uint32 n) {
	// make sure we get the create component
	if (!self.HasComponent<EntityCreatorComponent>())
		self.AddComponent<EntityCreatorComponent>();
	auto& creator = self.GetComponent<EntityCreatorComponent>();

	// we need the components to attach
	auto comps = m_attachcomponents;
	vec2 vel = m_defaultVelocity;
	vec2 pos = m_defaultPosition;

	// loop to instance each entity
	for (size_t i = 0; i < n; i++) {
		creator.Create([comps, vel, pos](ALC::Entity bullet) {
			// attach the required bullet body and sprite
			auto& bb = bullet.AddComponent<BulletBody>();
			bullet.AddComponent<SpriteComponent>();
			auto& tr = bullet.AddComponent<Transform2D>();

			bb.velocity = vel;
			tr.position = pos;

			// set the extra components if there are any
			if (comps) comps(bullet);
		});
	}
}

template<typename Callable>
inline void ALC::ShooterBehavior::Shoot(Entity self, const uint32 n, Callable callable) {
	// make sure we get the create component
	if (!self.HasComponent<EntityCreatorComponent>())
		self.AddComponent<EntityCreatorComponent>();
	auto& creator = self.GetComponent<EntityCreatorComponent>();

	// we need the components to attach
	auto comps = m_attachcomponents;
	vec2 vel = m_defaultVelocity;
	vec2 pos = m_defaultPosition;

	// loop to instance each entity
	for (size_t i = 0; i < n; i++) {
		creator.Create([comps, callable, vel, pos](ALC::Entity bullet) {
			// attach the required bullet body and sprite
			auto& bb = bullet.AddComponent<BulletBody>();
			bullet.AddComponent<SpriteComponent>();
			auto& tr = bullet.AddComponent<Transform2D>();

			bb.velocity = vel;
			tr.position = pos;

			// set the extra components if there are any
			if (comps) comps(bullet);

			// call the callable
			callable(bullet);
		});
	}
}

template<typename Callable>
inline void ALC::ShooterBehavior::ShootCircle(Entity self, const uint32 n, Callable callable) {
	// make sure we get the create component
	if (!self.HasComponent<EntityCreatorComponent>())
		self.AddComponent<EntityCreatorComponent>();
	auto& creator = self.GetComponent<EntityCreatorComponent>();

	// we need the components to attach
	auto comps = m_attachcomponents;
	vec2 pos = m_defaultPosition;

	// loop to instance each entity
	for (size_t i = 0; i < n; i++) {
		// the initial velocity of the bullet
		vec2 vel = glm::rotate(m_defaultVelocity,
							   glm::radians(360.0f / static_cast<float>(n)) * static_cast<float>(i));

		creator.Create([comps, callable, i, vel, pos](ALC::Entity bullet) {
			// attach the required bullet body and sprite
			auto& bb = bullet.AddComponent<BulletBody>();
			bullet.AddComponent<SpriteComponent>();
			auto& tr = bullet.AddComponent<Transform2D>();

			bb.velocity = vel;
			tr.position = pos;

			// set the extra components if there are any
			if (comps) comps(bullet);

			// call the callable
			callable(bullet);
		});
	}
}

template<typename Callable>
inline void ALC::ShooterBehavior::ShootRange(Entity self, const uint32 n, const float rangeInDegrees, Callable callable) {
	// make sure we get the create component
	if (!self.HasComponent<EntityCreatorComponent>())
		self.AddComponent<EntityCreatorComponent>();
	auto& creator = self.GetComponent<EntityCreatorComponent>();

	// we need the components to attach
	auto comps = m_attachcomponents;
	float initrange = glm::radians(-rangeInDegrees * 0.5f);
	float interval = static_cast<float>(rangeInDegrees) / static_cast<float>(n);
	vec2 pos = m_defaultPosition;

	// loop to instance each entity
	for (size_t i = 0; i < n; i++) {
		vec2 vel = glm::rotate(m_defaultVelocity, initrange +
							   glm::radians(interval * static_cast<float>(i) /* tmp fix -> */ + interval * 0.5f));

		creator.Create([comps, callable, vel, pos](ALC::Entity bullet) {
			// attach the required bullet body and sprite
			auto& bb = bullet.AddComponent<BulletBody>();
			bullet.AddComponent<SpriteComponent>();
			auto& tr = bullet.AddComponent<Transform2D>();

			bb.velocity = vel;
			tr.position = pos;

			// set the extra components if there are any
			if (comps) comps(bullet);

			// call the callable
			callable(bullet);
		});
	}
}
