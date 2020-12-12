//#include "..\Registry.hpp"

namespace ALC {

	///////////////////////////// Entity

	inline Entity::Entity()
		: m_entity(entt::null), m_registry(nullptr) { }

	inline bool Entity::IsValid() const {
		return m_entity != entt::null;
	}

	inline Entity::operator bool() const {
		return m_entity != entt::null;
	}

	template<typename... Component, typename>
	inline bool Entity::HasComponent() {
		return m_registry->__GetReg().has<Component...>(m_entity);
	}

	template<typename Component, typename... Args, typename>
	inline Component& Entity::AddComponent(Args&&... args) {
		if (HasComponent<Component>())
			return m_registry->__GetReg().get<Component>(m_entity);
		return m_registry->__GetReg().emplace<Component>(m_entity);
	}

	template<typename... Component, typename>
	inline decltype(auto) Entity::GetComponent() {
		return m_registry->__GetReg().get<Component...>(m_entity);
	}

	template<typename T, typename>
	inline T* Entity::AddBehavior() {
		if (!HasComponent<detail::BehaviorList>())
			AddComponent<detail::BehaviorList>();
		detail::BehaviorList& blist = GetComponent<detail::BehaviorList>();
		T* behavior = new T();
		blist.behaviors.push_back(behavior);
		////////////////////////////////behavior->__SetContext(m_entity, m_registry);
		behavior->Start(*this);
		return behavior;
	}

	template<typename T, typename>
	inline T* Entity::GetBehavior() {
		if (!HasComponent<detail::BehaviorList>())
			return nullptr;
		detail::BehaviorList& blist = GetComponent<detail::BehaviorList>();
		for (Behavior* b : blist.behaviors) {
			if (T* behavior = dynamic_cast<T>(b)) {
				return behavior;
			}
		}
		return nullptr;
	}

	template<typename T, typename>
	inline vector<T*> Entity::GetBehaviors() {
		if (!HasComponent<detail::BehaviorList>())
			return vector<T*>();
		vector<T*> behaviorsT;
		detail::BehaviorList& blist = GetComponent<detail::BehaviorList>();
		for (Behavior* b : blist.behaviors) {
			if (T* behavior = dynamic_cast<T>(b)) {
				behaviorsT.push_back(behavior);
			}
		}
		return behaviorsT;
	}

	inline Registry& Entity::GetRegistry() {
		return *m_registry;
	}

	inline Entity::Entity(entt::entity entity_, Registry* registry_)
		: m_entity(entity_), m_registry(registry_) { }

	inline entt::entity Entity::__GetEntt() {
		return m_entity;
	}


	///////////////////////////// Behavior

	inline Behavior::Behavior() : m_ID(-1), m_registry(nullptr) { }

	inline Behavior::~Behavior() { }

	inline void Behavior::Start(Entity e) { }
	inline void Behavior::Update(Entity e) { }
	inline void Behavior::LateUpdate(Entity e) { }
	inline void Behavior::OnDestroy(Entity e) { }

	inline Entity Behavior::GetEntity() {
		return m_registry->GetEntity(m_ID);
	}

	inline EntityID Behavior::GetEntityID() {
		return m_ID;
	}

	inline Registry& Behavior::GetRegistry() {
		return *m_registry;
	}

	inline void Behavior::__SetContext(EntityID ID_, Registry* reg_) {
		m_ID = ID_;
		m_registry = reg_;
	}


	///////////////////////////// EntityInfo

	inline EntityInfo::EntityInfo() { }

	inline EntityID EntityInfo::GetID() const {
		return m_ID;
	}

	inline EntityInfo::EntityInfo(EntityID ID_)
		: m_ID(ID_) { }


	///////////////////////////// Registry

	inline Registry::Registry()
		: m_entityIDCounter(0) { }

	inline Registry::~Registry() { DestroyAll(); }

	inline Entity Registry::Create() {
		EntityID ID(m_entityIDCounter++);
		entt::entity entity = m_registry.create();
		Entity e(entity, this);
		m_registry.emplace<EntityInfo>(entity, EntityInfo(ID));
		return e;
	}

	inline Entity Registry::GetEntity(EntityID id) {
		entt::entity e;

		m_registry.view<EntityInfo>().each(
			[&e, id](entt::entity entity, EntityInfo& b) {
			if (b.GetID() == id)
				e = entity;
		});

		return Entity(e, this);
	}

	inline Entity Registry::GetEntity(const string& name) {
		entt::entity e;

		m_registry.view<EntityInfo>().each(
			[&e, &name](entt::entity entity, EntityInfo& b) {
			if (b.name == name)
				e = entity;
		});

		return Entity(e, this);
	}

	inline void Registry::DestroyAll() {
		m_registry.view<detail::BehaviorList>().each(
			[this](entt::entity e, detail::BehaviorList& bl) {
			Entity entity(e, this);
			for (Behavior* b : bl.behaviors) {
				b->OnDestroy(entity);
				delete b;
			}
			bl.behaviors.clear();
		});
		m_registry.clear();
	}

	inline void ALC::Registry::Cleanup() {
		// destroy behaviors
		for (auto pr : m_behaviorsToDestroy) {
			m_registry.view<detail::BehaviorList>().each([pr, this] (entt::entity e, detail::BehaviorList& bl) {
				if (pr.first == e) {
					for (auto it = bl.behaviors.begin(); it != bl.behaviors.end(); ++it) {
						if ((*it) == pr.second) {
							(*it)->OnDestroy(Entity(pr.first, this));
							bl.behaviors.erase(it);
							break;
						}
					}
				}
			});
		}
		m_behaviorsToDestroy.clear();

		// destroy components
		for (auto destroybinding : m_componentsToDestroy) {
			destroybinding.second(&m_registry, destroybinding.first);
		}
		m_componentsToDestroy.clear();

		// destroy entities
		m_registry.destroy(m_entitiesToDestroy.begin(), m_entitiesToDestroy.end());
		m_entitiesToDestroy.clear();
	}

	inline void Registry::DestroyEntity(Entity e) {
		m_entitiesToDestroy.push_back(e.__GetEntt());
	}

	template<typename T, typename>
	inline void Registry::DestroyComponent(Entity e) {
		m_componentsToDestroy.push_back(CompDestroyPair(
			e.__GetEntt(),
			[](entt::registry* reg, entt::entity e) {
			reg->remove_if_exists<T>(e);
		}));
	}

	inline void Registry::DestroyBehavior(Behavior* behavior, Entity e) {
		m_behaviorsToDestroy.push_back(BehaviorPair(e.__GetEntt(), behavior));
	}

	template<typename... Components, typename Callable, typename>
	inline void Registry::ForeachComponent(Callable callable) {
		m_registry.view<Components...>().each(
			[callable, this](entt::entity e, Components&... comps) {
			Entity entity(e, this);
			callable(entity, comps...);
		});
	}

	template<typename Callable>
	inline void Registry::ForeachBehavior(Callable callable) {
		m_registry.view<detail::BehaviorList>()
			.each([callable, this](entt::entity e, detail::BehaviorList& bl) {
			Entity entity(e, this);
			callable(entity, bl);
		});
	}


}
