

inline void ALC::ShooterBehavior::SetDefaultVelocity(const vec2& defaultVelocity) {
	m_defaultVelocity = defaultVelocity;
}

inline void ALC::ShooterBehavior::SetDefaultPosition(const vec2& defaultPosition) {
	m_defaultPosition = defaultPosition;
}

inline void ALC::ShooterBehavior::SetDefaultCollisionmask(const Layermask32& defaultCollisionmask) {
	m_defaultCollisionmask = defaultCollisionmask;
}

inline void ALC::ShooterBehavior::SetDefaultSpriteLayer(const int32 layer) {
	m_defaultSprlayer = layer;
}

inline void ALC::ShooterBehavior::InitBullet(Transform2D& tr, BulletBody& bb, SpriteComponent& spr) {
	bb.velocity = m_defaultVelocity;
	tr.position = m_defaultPosition;
	bb.mask = m_defaultCollisionmask;
	spr.layer = m_defaultSprlayer;
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
inline void ALC::ShooterBehavior::Shoot(Entity self, const uint32 n, Callable callable, BulletTypes<Components...>) {
	// make sure we get the create component
	if (!self.HasComponent<EntityCreatorComponent>())
		self.AddComponent<EntityCreatorComponent>();
	auto& creator = self.GetComponent<EntityCreatorComponent>();

	uint32 count = 0;

	// get n bullets with the given components
	GetRegistry().ForeachComponent<BulletBody, Transform2D, SpriteComponent, Components...>(
		[this, &count, n, callable](Entity e, BulletBody& bb, Transform2D& tr, SpriteComponent& spr, Components&... comps) {
		if (!bb.isSimulated && count != n) {
			Detail::ReconstructEach(bb, spr, comps...);
			InitBullet(tr, bb, spr);
			callable(e);
			count++;
		}
	});

	while (count < n) {
		creator.Create([this, &count, callable](Entity e) {
			auto& tr = e.AddComponent<Transform2D>();
			auto& bb = e.AddComponent<BulletBody>();
			auto& spr = e.AddComponent<SpriteComponent>();
			(e.AddComponent<Components>(), ...);
			InitBullet(tr, bb, spr);
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

	// get n bullets with the given components
	GetRegistry().ForeachComponent<BulletBody, Transform2D, SpriteComponent, Components...>(
		[this, &count, n, callable, angleInDegrees](Entity e, BulletBody& bb, Transform2D& tr, SpriteComponent& spr, Components&... comps) {
		if (!bb.isSimulated && count < n) {
			Detail::ReconstructEach(bb, spr, comps...);
			InitBullet(tr, bb, spr);
			bb.velocity = glm::rotate(bb.velocity, glm::radians(angleInDegrees));
			callable(e);
			count++;
		}
	});

	while (count < n) {
		creator.Create([this, &count, callable, angleInDegrees](Entity e) {
			auto& tr = e.AddComponent<Transform2D>();
			auto& bb = e.AddComponent<BulletBody>();
			auto& spr = e.AddComponent<SpriteComponent>();
			InitBullet(tr, bb, spr);
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

	// get n bullets with the given components
	GetRegistry().ForeachComponent<BulletBody, Transform2D, SpriteComponent, Components...>(
		[this, &count, n, callable](Entity e, BulletBody& bb, Transform2D& tr, SpriteComponent& spr, Components&... comps) {
		if (!bb.isSimulated && count != n) {
			Detail::ReconstructEach(bb, spr, comps...);
			InitBullet(tr, bb, spr);
			bb.velocity = glm::rotate(m_defaultVelocity,
									  glm::radians(360.0f / static_cast<float>(n)) * static_cast<float>(count));
			callable(e);
			count++;
		}
	});

	while (count < n) {
		creator.Create([this, &count, callable](Entity e) {
			auto& tr = e.AddComponent<Transform2D>();
			auto& bb = e.AddComponent<BulletBody>();
			auto& spr = e.AddComponent<SpriteComponent>();
			InitBullet(tr, bb, spr);
			bb.velocity = glm::rotate(m_defaultVelocity,
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
	float initrange = glm::radians(-rangeInDegrees * 0.5f);
	float interval = static_cast<float>(rangeInDegrees) / static_cast<float>(n);

	// get n bullets with the given components
	GetRegistry().ForeachComponent<BulletBody, Transform2D, SpriteComponent, Components...>(
		[this, &count, n, callable, initrange, interval](Entity e, BulletBody& bb, Transform2D& tr, SpriteComponent& spr, Components&... comps) {
		if (!bb.isSimulated && count != n) {
			Detail::ReconstructEach(bb, spr, comps...);
			InitBullet(tr, bb, spr);
			bb.velocity = glm::rotate(m_defaultVelocity, initrange +
									  glm::radians(interval * static_cast<float>(count) /* tmp fix -> */ + interval * 0.5f));
			callable(e);
			count++;
		}
	});

	while (count < n) {
		creator.Create([this, &count, callable, initrange, interval](Entity e) {
			auto& tr = e.AddComponent<Transform2D>();
			auto& bb = e.AddComponent<BulletBody>();
			auto& spr = e.AddComponent<SpriteComponent>();
			InitBullet(tr, bb, spr);
			bb.velocity = glm::rotate(m_defaultVelocity, initrange +
									  glm::radians(interval * static_cast<float>(count) /* tmp fix -> */ + interval * 0.5f));
			(e.AddComponent<Components>(), ...);
			callable(e);
		});
		count++;
	}

}
