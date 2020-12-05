#ifndef ALC_ENTITIES_ENTITY_REGISTRY_HPP
#define ALC_ENTITIES_ENTITY_REGISTRY_HPP
#include <entt\entt.hpp>
#include "../General.hpp"

namespace ALC {

	// mark all components using this tag
	#define ALC_COMPONENT final

	#pragma region DETAIL

	class Behavior;

	namespace detail {
		struct BehaviorList ALC_COMPONENT {
			using BehaviorRef = std::unique_ptr<Behavior>;
			vector<BehaviorRef> list;
		};
	}

	#pragma endregion

	struct Entity;

	class EntityRegistry final {
	public:

		EntityRegistry();
		~EntityRegistry();

		// events
		void Update();
		void LateUpdate();
		void Cleanup();

		// sets the init function for entity creation
		// function is called with all CreateEntity calls
		void RegInitFunc(void(*initEntityFunc_)(Entity));

		// creates and returns an entity
		Entity CreateEntity();

		template<typename... Component, typename Func>
		void Foreach(Func f);
		template<typename... Component, typename... Exclude>
		decltype(auto) View(entt::exclude_t<Exclude...> ex = {});

		void DestroyEntity(Entity entity);
		template<typename Component> void DestroyComponent(Entity entity);
		void DestroyBehavior(Entity entity, const Behavior* beh);

	private:
		using CompDestroyBinding = void(*)(entt::registry*, entt::entity);
		using CompDestroyPair = pair<entt::entity, CompDestroyBinding>;
		using BehaviorPair = pair<entt::entity, const Behavior*>;

		void(*initEntityFunc)(Entity);
		entt::registry reg;
		std::vector<entt::entity> entitiesToDestroy;
		std::vector<CompDestroyPair> componentsToDestroy;
		std::vector<BehaviorPair> behaviorsToDestroy;
	public:
		entt::registry* __GetReg() { return &reg; }
	};

	struct Entity final {

		Entity() : e(entt::null), reg(nullptr) { }

		bool IsValid() const { return e != entt::null; }
		operator bool() const { return e != entt::null; }

		template<typename Component> bool HasComponent();
		template<typename Component, typename... Args> Component& AddComponent(Args&&... args);
		template<typename... Component> decltype(auto) GetComponent();

		template<typename T> T* AddBehavior();
		template<typename T> T* GetBehavior();

		EntityRegistry* GetRegistry() const { return reg; }

	public:
		Entity(entt::entity e_, EntityRegistry* reg_)
			: e(e_), reg(reg_) { }
		entt::entity __GetEntt() const { return e; }
	private:
		entt::entity e;
		EntityRegistry* reg;
	};

	class Behavior {
	public:
		Behavior() { }
		virtual ~Behavior() = 0 { }

		virtual void Start(Entity e) { }
		virtual void Update(Entity e) { }
		virtual void LateUpdate(Entity e) { }
		virtual void OnDestroy(Entity e) { }
	};

	#pragma region Implementation for template functions

	template<typename Component>
	inline bool Entity::HasComponent() {
		return reg->__GetReg()->has<Component>(e);
	}

	template<typename Component, typename ...Args>
	inline Component& Entity::AddComponent(Args && ...args) {
		if (HasComponent<Component>())
			return reg->__GetReg()->get<Component>(e);
		return reg->__GetReg()->emplace<Component>(e, args...);
	}

	template<typename ...Component>
	inline decltype(auto) Entity::GetComponent() {
		return reg->__GetReg()->get<Component...>(e);
	}

	template<typename T>
	inline T* Entity::AddBehavior() {
		if constexpr (std::is_base_of_v<Behavior, T>) {
			if (!HasComponent<detail::BehaviorList>())
				AddComponent<detail::BehaviorList>();
			auto& blist = GetComponent<detail::BehaviorList>();
			T* t = new T();
			Behavior* b = t;
			blist.list.push_back(detail::BehaviorList::BehaviorRef(b));
			b->Start(*this);
			return t;
			return nullptr;
		}
		else {
			ALC_DEBUG_ERROR("Could not add behaviour of type");
			return nullptr;
		}
	}

	template<typename T>
	inline T* Entity::GetBehavior() {
		if constexpr (std::is_base_of_v<Behavior, T>) {
			if (!HasComponent<detail::BehaviorList>())
				return nullptr;
			detail::BehaviorList& blist = GetComponent<detail::BehaviorList>();
			// search for behaviour
			for (Ref<Behavior>* b : blist.list) {
				if (T* t = dynamic_cast<T*>(b->get())) {
					return t;
				}
			}
			ALC_DEBUG_ERROR("There was no attached Behavior of type");
			return nullptr;
		}
		else {
			ALC_DEBUG_ERROR("Could not add behaviour of type");
			return nullptr;
		}
	}

	template<typename ...Component, typename Func>
	inline void EntityRegistry::Foreach(Func f) {
		reg.view<Component...>().each(f);
	}

	template<typename ...Component, typename ...Exclude>
	inline decltype(auto) EntityRegistry::View(entt::exclude_t<Exclude...> ex) {
		return reg.view<Component...>(ex);
	}

	template<typename Component>
	inline void EntityRegistry::DestroyComponent(Entity entity) {
		if (std::is_same_v<Component, detail::BehaviorList>)
			return;
		CompDestroyBinding lambda = [] (entt::registry* reg, entt::entity e) {
			reg->remove<Component>(e);
		};
		componentsToDestroy.push_back(std::make_pair(entity.__GetEntt(), lambda));
	}

	#pragma endregion

}

#endif // !ALC_ENTITIES_ENTITY_REGISTRY_HPP