#ifndef ALC_ENTITIES_REGISTRY_HPP
#define ALC_ENTITIES_REGISTRY_HPP
#include <entt\entt.hpp>
#include "EntityID.hpp"
#include <glm\glm.hpp>
#include "detail\RegistryDetail.hpp"

namespace ALC {

	// represents an entity in the registry
	// becomes invalid if the Registry::EndFrame() is called
	struct Entity final {

		Entity();

		// checks if this is a valid entity
		bool IsValid() const;

		// checks if this is a valid entity
		operator bool() const;

		// returns if component exists on entity
		template<typename... Component, typename = detail::are_components_t<Component...>>
		bool HasComponent();

		// adds a component and initializes it with the args
		// if it already exists then it returns the component as-is
		template<typename Component, typename... Args>
		Component& AddComponent(Args&&... args);

		// gets a component if it already exists
		// can return multiple components
		template<typename... Component, typename = detail::are_components_t<Component...>>
		decltype(auto) GetComponent();

		// creates a behavior of type
		template<typename T, typename = detail::is_behavior_t<T>>
		T* AddBehavior();

		// returns a pointer to the behavior of type
		// returns nullptr if it's not found
		template<typename T, typename = detail::is_behavior_t<T>>
		T* GetBehavior();

		// returns X number of behaviors of type
		template<typename T, typename = detail::is_behavior_t<T>>
		vector<T*> GetBehaviors();

		Registry& GetRegistry();


		explicit Entity(entt::entity entity_, Registry* registry_) ALC_INTERNAL;
		entt::entity __GetEntt() ALC_INTERNAL;
	private:
		entt::entity m_entity;
		Registry* m_registry;
	};

	class Behavior {
	public:

		Behavior();
		virtual ~Behavior() = 0;

		// virtual functions

		virtual void Start(Entity e);
		virtual void Update(Entity e);
		virtual void LateUpdate(Entity e);
		virtual void OnDestroy(Entity e);

		// returns the entity
		// slow since it needs to search for the entity
		Entity GetEntity();

		// returns the entity id
		EntityID GetEntityID();

		// returns the registry that this behavior is attached to
		Registry& GetRegistry();


		void __SetContext(EntityID ID_, Registry* reg_) ALC_INTERNAL;
	private:
		EntityID m_ID;
		Registry* m_registry;
	};

	struct EntityInfo final : ALC_Component {

		EntityInfo();

		string name;
		EntityID GetID() const;


		explicit EntityInfo(EntityID ID_) ALC_INTERNAL;
	private:
		EntityID m_ID;
	};

	struct Transform final : ALC_Component {

		glm::vec3 position;
		glm::vec3 scale;
		glm::vec3 rotation;

	};

	class Registry final {
		ALC_NO_COPY(Registry)
	public:

		Registry();
		~Registry();

		// creates a new entity
		// new entities contain a Transform and EntityInfo
		Entity Create();

		// returns the entity by its ID
		Entity GetEntity(EntityID id);

		// returns the entity by its name
		Entity GetEntity(const string& name);

		// destroys all entities
		void DestroyAll();

		// update event
		void UpdateBehaviors();

		// lateupdate event
		void LateUpdateBehaviors();

		// cleans up
		// must be called for marked destructibles to be destroyed
		void Cleanup();

		// marks the entity to be destroyed
		void DestroyEntity(Entity e);

		// marks the component to be destroyed
		template<typename T, typename = detail::is_component_t<T>>
		void DestroyComponent(Entity e);

		// marks the behavior to be destroyed
		void DestroyBehavior(Behavior* behavior, Entity e);

		// iterate through the components
		template<typename... Components, typename Callable>
		void ForeachComponent(Callable callable);

		// iterate through the behaviors
		template<typename Callable>
		void ForeachBehavior(Callable callable);


		entt::registry& __GetReg() ALC_INTERNAL;
	private:

		entt::registry m_registry;
		uint64 m_entityIDCounter;

		using CompDestroyBinding = void(*)(entt::registry*, entt::entity);
		using CompDestroyPair = pair<entt::entity, CompDestroyBinding>;
		using BehaviorPair = pair<entt::entity, const Behavior*>;

		std::vector<entt::entity> m_entitiesToDestroy;
		std::vector<CompDestroyPair> m_componentsToDestroy;
		std::vector<BehaviorPair> m_behaviorsToDestroy;
	};

}

// includes the definitions for all the inline functions
#include "detail\Registry.inl"

#endif // !ALC_ENTITIES_REGISTRY_HPP