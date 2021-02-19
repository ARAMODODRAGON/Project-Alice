

inline void ALC::ShooterBehavior::SetDefaultVelocity(const vec2& defaultVelocity) {
	m_defaults.velocity = defaultVelocity;
}

inline void ALC::ShooterBehavior::SetDefaultPosition(const vec2& defaultPosition) {
	m_defaults.position = defaultPosition;
}

inline void ALC::ShooterBehavior::SetDefaultCollisionmask(const Layermask32& defaultCollisionmask) {
	m_defaults.collisionmask = defaultCollisionmask;
}

inline void ALC::ShooterBehavior::SetDefaultSpriteLayer(const int32 layer) {
	m_defaults.sprlayer = layer;
}

inline void ALC::ShooterBehavior::InitBullet(Default def, Transform2D& tr, BulletBody& bb, SpriteComponent& spr) {
	bb.velocity = def.velocity;
	tr.position = def.position;
	bb.mask = def.collisionmask;
	spr.layer = def.sprlayer;
}

namespace ALC {
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
}

template<typename Callable, typename... Components>
inline void ALC::ShooterBehavior::Shoot(Entity self, const uint32 n, Callable callable, BulletTypes<Components...> bt) {
	// make sure we get the create component
	if (!self.HasComponent<EntityCreatorComponent>())
		self.AddComponent<EntityCreatorComponent>();
	auto& creator = self.GetComponent<EntityCreatorComponent>();

	uint32 count = 0;
	Default def = m_defaults;

	// get n bullets with the given components
	GetRegistry().ForeachComponent<BulletBody, Transform2D, SpriteComponent, Components...>(
		[this, &count, n, callable, def](Entity e, BulletBody& bb, Transform2D& tr, SpriteComponent& spr, Components&... comps) {
		if (!bb.isSimulated && count != n) {
			Detail::ReconstructEach(bb, spr, comps...);
			InitBullet(def, tr, bb, spr);
			callable(e);
			count++;
		}
	});

	while (count < n) {
		creator.Create([this, &count, callable, bt, def](Entity e) {
			auto& tr = e.AddComponent<Transform2D>();
			auto& bb = e.AddComponent<BulletBody>();
			auto& spr = e.AddComponent<SpriteComponent>();
			(e.AddComponent<Components>(), ...);
			InitBullet(def, tr, bb, spr);
			callable(e);
		});
		count++;
	}

}

template<typename Callable, typename ...Components>
inline void ALC::ShooterBehavior::Shoot(Entity self, const uint32 n, const float angleInDegrees, Callable callable, BulletTypes<Components...>) {
	// make sure we get the create component
	if (!self.HasComponent<EntityCreatorComponent>())
		self.AddComponent<EntityCreatorComponent>();
	auto& creator = self.GetComponent<EntityCreatorComponent>();

	uint32 count = 0;
	Default def = m_defaults;

	// get n bullets with the given components
	GetRegistry().ForeachComponent<BulletBody, Transform2D, SpriteComponent, Components...>(
		[this, &count, n, callable, angleInDegrees, def](Entity e, BulletBody& bb, Transform2D& tr, SpriteComponent& spr, Components&... comps) {
		if (!bb.isSimulated && count < n) {
			Detail::ReconstructEach(bb, spr, comps...);
			InitBullet(def, tr, bb, spr);
			bb.velocity = glm::rotate(bb.velocity, glm::radians(angleInDegrees));
			callable(e);
			count++;
		}
	});

	while (count < n) {
		creator.Create([this, &count, callable, angleInDegrees, def](Entity e) {
			auto& tr = e.AddComponent<Transform2D>();
			auto& bb = e.AddComponent<BulletBody>();
			auto& spr = e.AddComponent<SpriteComponent>();
			InitBullet(def, tr, bb, spr);
			bb.velocity = glm::rotate(bb.velocity, glm::radians(angleInDegrees));
			(e.AddComponent<Components>(), ...);
			callable(e);
		});
		count++;
	}

}

template<typename Callable, typename ...Components>
inline void ALC::ShooterBehavior::ShootCircle(Entity self, const uint32 n, Callable callable, BulletTypes<Components...>) {
	// make sure we get the create component
	if (!self.HasComponent<EntityCreatorComponent>())
		self.AddComponent<EntityCreatorComponent>();
	auto& creator = self.GetComponent<EntityCreatorComponent>();

	uint32 count = 0;
	Default def = m_defaults;

	// get n bullets with the given components
	GetRegistry().ForeachComponent<BulletBody, Transform2D, SpriteComponent, Components...>(
		[this, &count, n, callable, def](Entity e, BulletBody& bb, Transform2D& tr, SpriteComponent& spr, Components&... comps) {
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
		creator.Create([this, &count, callable, def](Entity e) {
			auto& tr = e.AddComponent<Transform2D>();
			auto& bb = e.AddComponent<BulletBody>();
			auto& spr = e.AddComponent<SpriteComponent>();
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
inline void ALC::ShooterBehavior::ShootRange(Entity self, const uint32 n, const float rangeInDegrees, Callable callable, BulletTypes<Components...>) {
	// make sure we get the create component
	if (!self.HasComponent<EntityCreatorComponent>())
		self.AddComponent<EntityCreatorComponent>();
	auto& creator = self.GetComponent<EntityCreatorComponent>();

	uint32 count = 0;
	Default def = m_defaults;
	float initrange = glm::radians(-rangeInDegrees * 0.5f);
	float interval = static_cast<float>(rangeInDegrees) / static_cast<float>(n);

	// get n bullets with the given components
	GetRegistry().ForeachComponent<BulletBody, Transform2D, SpriteComponent, Components...>(
		[this, &count, n, callable, initrange, interval, def](Entity e, BulletBody& bb, Transform2D& tr, SpriteComponent& spr, Components&... comps) {
		if (!bb.isSimulated && count != n) {
			Detail::ReconstructEach(bb, spr, comps...);
			InitBullet(def, tr, bb, spr);
			bb.velocity = glm::rotate(def.velocity, initrange +
									  glm::radians(interval * static_cast<float>(count) /* tmp fix -> */ + interval * 0.5f));
			callable(e);
			count++;
		}
	});

	while (count < n) {
		creator.Create([this, &count, callable, initrange, interval, def](Entity e) {
			auto& tr = e.AddComponent<Transform2D>();
			auto& bb = e.AddComponent<BulletBody>();
			auto& spr = e.AddComponent<SpriteComponent>();
			InitBullet(def, tr, bb, spr);
			bb.velocity = glm::rotate(def.velocity, initrange +
									  glm::radians(interval * static_cast<float>(count) /* tmp fix -> */ + interval * 0.5f));
			(e.AddComponent<Components>(), ...);
			callable(e);
		});
		count++;
	}

}
