/*
* MIT License
*
* Copyright (c) 2021 Domara Shlimon
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
//#include "..\Registry.hpp"
//#include "..\Registry.hpp"

namespace ALC {

	///////////////////////////// Entity

	inline Entity::Entity()
		: m_entity(entt::null), m_registry(nullptr) { }

	inline Entity::Entity(std::nullptr_t)
		: m_entity(entt::null), m_registry(nullptr) { }

	inline bool Entity::IsValid() const {
		return m_entity != entt::null;
	}

	inline Entity::operator bool() const {
		return m_entity != entt::null;
	}

	template<typename... Component>
	inline bool Entity::HasComponent() {
		return m_registry->__GetReg().has<Component...>(m_entity);
	}

	template<typename Component, typename... Args>
	inline Component& Entity::AddComponent(Args&&... args) {
		if (HasComponent<Component>())
			return m_registry->__GetReg().get<Component>(m_entity);
		return m_registry->__GetReg().emplace_or_replace<Component>(m_entity);
	}

	template<typename... Component>
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
		auto& entityinfo = GetComponent<EntityInfo>();
		behavior->__SetContext(entityinfo.GetID(), m_registry);
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
	inline void Behavior::Update(Entity e, Timestep ts) { }
	inline void Behavior::LateUpdate(Entity e, Timestep ts) { }
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
		: m_validState(true), m_entityIDCounter(0) { }

	inline Registry::~Registry() { DestroyAll(); }

	inline Entity Registry::Create() {
		//ALC_ASSERT(m_validState, "cannot create while iterating through the registry");
		EntityID ID(m_entityIDCounter++);
		entt::entity entity = m_registry.create();
		Entity e(entity, this);
		m_registry.emplace_or_replace<EntityInfo>(entity, EntityInfo(ID));
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
		//ALC_ASSERT(m_validState, "cannot destroy while iterating through the registry");
		m_validState = false;
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
		m_validState = true;
	}

	inline void Registry::UpdateBehaviors(Timestep t) {
		//ALC_ASSERT(m_validState, "cannot have multiple levels of iteration");
		m_validState = false;
		m_registry.view<detail::BehaviorList>().each(
			[this, t](entt::entity e, detail::BehaviorList& bl) {
			Entity entity(e, this);
			for (Behavior* b : bl.behaviors) {
				b->Update(entity, t);
			}
		});
		m_validState = true;
	}

	inline void Registry::LateUpdateBehaviors(Timestep t) {
		//ALC_ASSERT(m_validState, "cannot have multiple levels of iteration");
		m_validState = false;
		m_registry.view<detail::BehaviorList>().each(
			[this, t](entt::entity e, detail::BehaviorList& bl) {
			Entity entity(e, this);
			for (Behavior* b : bl.behaviors) {
				b->LateUpdate(entity, t);
			}
		});
		m_validState = true;
	}

	inline void ALC::Registry::Cleanup() {
		//ALC_ASSERT(m_validState, "cannot destroy while iterating through registry");
		m_validState = false;
		// destroy behaviors
		for (auto pr : m_behaviorsToDestroy) {
			m_registry.view<detail::BehaviorList>().each([pr, this](entt::entity e, detail::BehaviorList& bl) {
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
		m_validState = true;
	}

	inline void Registry::DestroyEntity(Entity e) {
		m_entitiesToDestroy.push_back(e.__GetEntt());
	}

	template<typename ...T>
	inline void Registry::StepSystem(Timestep ts, ISystem<T...>& system) {
		//ALC_ASSERT(m_validState, "cannot have multiple levels of iteration");
		m_validState = false;

		// foreach entity with matching components in systems
		auto view = m_registry.view<T...>();
		view.each(
			[&system, &ts, this](entt::entity ent, T&... params) {
			Entity e(ent, this);
			system.Step(ts, e, params...);
		});

		m_validState = true;
	}

	template<typename T>
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

	template<typename... Components, typename Callable>
	inline void Registry::ForeachComponent(Callable callable) {
		//ALC_ASSERT(m_validState, "cannot have multiple levels of iteration");
		m_validState = false;
		m_registry.view<Components...>().each(
			[callable, this](entt::entity e, Components&... comps) {
			Entity entity(e, this);
			callable(entity, comps...);
		});
		m_validState = true;
	}

	template<typename Callable>
	inline void Registry::ForeachBehavior(Callable callable) {
		//ALC_ASSERT(m_validState, "cannot have multiple levels of iteration");
		m_validState = false;
		m_registry.view<detail::BehaviorList>()
			.each([callable, this](entt::entity e, detail::BehaviorList& bl) {
			Entity entity(e, this);
			callable(entity, bl);
		});
		m_validState = true;
	}

	inline entt::registry& Registry::__GetReg() {
		return m_registry;
	}

}
