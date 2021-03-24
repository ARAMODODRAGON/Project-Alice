

inline void ShooterBehavior::SetDefaultVelocity(const ALC::vec2& defaultVelocity) {
	m_defaults.velocity = defaultVelocity;
}

inline void ShooterBehavior::SetDefaultPosition(const ALC::vec2& defaultPosition) {
	m_defaults.position = defaultPosition;
}

inline void ShooterBehavior::SetDefaultCollisionmask(const ALC::Layermask32& defaultCollisionmask) {
	m_defaults.collisionmask = defaultCollisionmask;
}

inline void ShooterBehavior::SetDefaultSpriteLayer(const ALC::int32 layer) {
	m_defaults.sprlayer = layer;
}

inline void ShooterBehavior::InitBullet(Default def, ALC::Transform2D& tr, BulletBody& bb, ALC::SpriteComponent& spr) {
	bb.velocity = def.velocity;
	tr.position = def.position;
	bb.mask = def.collisionmask;
	spr.layer = def.sprlayer;
}

namespace Detail {
	template<typename T, typename... Params>
	struct _ReconstructEach {
		static void Each(T& first, Params&... params) {
			first.~T();
			new (&first) T();

			if constexpr (sizeof...(Params) > 0) {
				_ReconstructEach<Params...>::Each(params...);
			}
		}
	};

	template<typename... T>
	void ReconstructEach(T&... params) {
		_ReconstructEach<T...>::Each(params...);
	}
}

template<typename Callable, typename... Components>
inline void ShooterBehavior::Shoot(ALC::Entity self, const ALC::uint32 n, Callable callable, BulletTypes<Components...> bt) {
	// make sure we get the create component
	if (!self.HasComponent<ALC::EntityCreatorComponent>())
		self.AddComponent<ALC::EntityCreatorComponent>();
	auto& creator = self.GetComponent<ALC::EntityCreatorComponent>();

	ALC::uint32 count = 0;
	Default def = m_defaults;

	// get n bullets with the given components
	GetRegistry().ForeachComponent<BulletBody, ALC::Transform2D, ALC::SpriteComponent, Components...>(
		[this, &count, n, callable, def](ALC::Entity e, BulletBody& bb, ALC::Transform2D& tr, ALC::SpriteComponent& spr, Components&... comps) {
		if (!bb.isSimulated && count != n) {
			Detail::ReconstructEach(bb, spr, comps...);
			InitBullet(def, tr, bb, spr);
			callable(e);
			count++;
		}
	});

	while (count < n) {
		creator.Create([this, &count, callable, bt, def](ALC::Entity e) {
			auto& tr = e.AddComponent<ALC::Transform2D>();
			auto& bb = e.AddComponent<BulletBody>();
			auto& spr = e.AddComponent<ALC::SpriteComponent>();
			(e.AddComponent<Components>(), ...);
			InitBullet(def, tr, bb, spr);
			callable(e);
		});
		count++;
	}

}

template<typename Callable, typename ...Components>
inline void ShooterBehavior::Shoot(ALC::Entity self, const ALC::uint32 n, const float angleInDegrees, Callable callable, BulletTypes<Components...>) {
	// make sure we get the create component
	if (!self.HasComponent<ALC::EntityCreatorComponent>())
		self.AddComponent<ALC::EntityCreatorComponent>();
	auto& creator = self.GetComponent<ALC::EntityCreatorComponent>();

	ALC::uint32 count = 0;
	Default def = m_defaults;

	// get n bullets with the given components
	GetRegistry().ForeachComponent<BulletBody, ALC::Transform2D, ALC::SpriteComponent, Components...>(
		[this, &count, n, callable, angleInDegrees, def](ALC::Entity e, BulletBody& bb, ALC::Transform2D& tr, ALC::SpriteComponent& spr, Components&... comps) {
		if (!bb.isSimulated && count < n) {
			Detail::ReconstructEach(bb, spr, comps...);
			InitBullet(def, tr, bb, spr);
			bb.velocity = glm::rotate(bb.velocity, glm::radians(angleInDegrees));
			callable(e);
			count++;
		}
	});

	while (count < n) {
		creator.Create([this, &count, callable, angleInDegrees, def](ALC::Entity e) {
			auto& tr = e.AddComponent<ALC::Transform2D>();
			auto& bb = e.AddComponent<BulletBody>();
			auto& spr = e.AddComponent<ALC::SpriteComponent>();
			InitBullet(def, tr, bb, spr);
			bb.velocity = glm::rotate(bb.velocity, glm::radians(angleInDegrees));
			(e.AddComponent<Components>(), ...);
			callable(e);
		});
		count++;
	}

}

template<typename Callable, typename ...Components>
inline void ShooterBehavior::ShootCircle(ALC::Entity self, const ALC::uint32 n, Callable callable, BulletTypes<Components...>) {
	// make sure we get the create component
	if (!self.HasComponent<ALC::EntityCreatorComponent>())
		self.AddComponent<ALC::EntityCreatorComponent>();
	auto& creator = self.GetComponent<ALC::EntityCreatorComponent>();

	ALC::uint32 count = 0;
	Default def = m_defaults;

	// get n bullets with the given components
	GetRegistry().ForeachComponent<BulletBody, ALC::Transform2D, ALC::SpriteComponent, Components...>(
		[this, &count, n, callable, def](ALC::Entity e, BulletBody& bb, ALC::Transform2D& tr, ALC::SpriteComponent& spr, Components&... comps) {
		if (!bb.isSimulated && count != n) {
			Detail::ReconstructEach(bb, spr, comps...);
			InitBullet(def, tr, bb, spr);
			bb.velocity = glm::rotate(def.velocity,
									  glm::radians(360.0f / static_cast<float>(n)) * static_cast<float>(count));
			callable(e);
			count++;
		}
	});

	while (count < n) {
		creator.Create([this, count, n, callable, def](ALC::Entity e) {
			auto& tr = e.AddComponent<ALC::Transform2D>();
			auto& bb = e.AddComponent<BulletBody>();
			auto& spr = e.AddComponent<ALC::SpriteComponent>();
			InitBullet(def, tr, bb, spr);
			bb.velocity = glm::rotate(def.velocity,
									  glm::radians(360.0f / static_cast<float>(n)) * static_cast<float>(count));
			(e.AddComponent<Components>(), ...);
			callable(e);
		});
		count++;
	}

}

template<typename Callable, typename ...Components>
inline void ShooterBehavior::ShootRange(ALC::Entity self, const ALC::uint32 n, const float rangeInDegrees, Callable callable, BulletTypes<Components...>) {
	// make sure we get the create component
	if (!self.HasComponent<ALC::EntityCreatorComponent>())
		self.AddComponent<ALC::EntityCreatorComponent>();
	auto& creator = self.GetComponent<ALC::EntityCreatorComponent>();

	ALC::uint32 count = 0;
	Default def = m_defaults;
	float initrange = 0.0f;
	float interval = 0.0f;
	if (n > 1) {
		initrange = glm::radians(-rangeInDegrees * 0.5f);
		interval = static_cast<float>(rangeInDegrees) / static_cast<float>(n - 1);
	}

	// get n bullets with the given components
	GetRegistry().ForeachComponent<BulletBody, ALC::Transform2D, ALC::SpriteComponent, Components...>(
		[this, &count, n, callable, initrange, interval, def](ALC::Entity e, BulletBody& bb, ALC::Transform2D& tr, ALC::SpriteComponent& spr, Components&... comps) {
		if (!bb.isSimulated && count != n) {
			Detail::ReconstructEach(bb, spr, comps...);
			InitBullet(def, tr, bb, spr);
			bb.velocity = glm::rotate(def.velocity, initrange +
									  glm::radians(interval * static_cast<float>(count)));
			callable(e);
			count++;
		}
	});

	while (count < n) {
		creator.Create([this, count, callable, initrange, interval, def](ALC::Entity e) {
			auto& tr = e.AddComponent<ALC::Transform2D>();
			auto& bb = e.AddComponent<BulletBody>();
			auto& spr = e.AddComponent<ALC::SpriteComponent>();
			InitBullet(def, tr, bb, spr);
			bb.velocity = glm::rotate(def.velocity, initrange +
									  glm::radians(interval * static_cast<float>(count)));
			(e.AddComponent<Components>(), ...);
			callable(e);
		});
		count++;
	}

}
