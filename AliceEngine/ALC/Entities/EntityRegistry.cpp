#include "EntityRegistry.hpp"

namespace ALC {

	EntityRegistry::EntityRegistry() : initEntityFunc(nullptr) {

	}

	EntityRegistry::~EntityRegistry() {
		// destroy all behaviors
		Foreach<detail::BehaviorList>([this] (entt::entity e, detail::BehaviorList& bl) {
			Entity entity(e, this);
			for (detail::BehaviorList::BehaviorRef& b : bl.list) {
				b->OnDestroy(entity);
			}
			bl.list.clear();
		});
	}

	void EntityRegistry::Update() {
		Foreach<detail::BehaviorList>([this] (entt::entity e, detail::BehaviorList& bl){
			Entity entity(e, this);
			for (auto& beh : bl.list)
				beh.get()->Update(entity);
		});
	}

	void EntityRegistry::LateUpdate() {
		Foreach<detail::BehaviorList>([this] (entt::entity e, detail::BehaviorList& bl){
			Entity entity(e, this);
			for (auto& beh : bl.list)
				beh.get()->LateUpdate(entity);
		});
	}

	void EntityRegistry::Cleanup() {
		// destroy behaviors
		for (auto pr : behaviorsToDestroy) {
			reg.view<detail::BehaviorList>().each([pr, this] (entt::entity e, detail::BehaviorList& bl) {
				if (pr.first == e) {
					for (auto it = bl.list.begin(); it != bl.list.end(); ++it) {
						if (it->get() == pr.second) {
							(*it)->OnDestroy(Entity(pr.first, this));
							bl.list.erase(it);
							break;
						}
					}
				}
			});
		}
		behaviorsToDestroy.clear();

		// destroy components
		for (auto destroybinding : componentsToDestroy) {
			destroybinding.second(&reg, destroybinding.first);
		}
		componentsToDestroy.clear();

		// destroy entities
		reg.destroy(entitiesToDestroy.begin(), entitiesToDestroy.end());
		entitiesToDestroy.clear();
	}

	void EntityRegistry::RegInitFunc(void(*initEntityFunc_)(Entity)) {
		initEntityFunc = initEntityFunc_;
	}

	Entity EntityRegistry::CreateEntity() {
		entt::entity e = reg.create();
		Entity entity(e, this);
		if (initEntityFunc) initEntityFunc(entity);
		return entity;
	}

	void EntityRegistry::DestroyEntity(Entity entity) {
		entitiesToDestroy.push_back(entity.__GetEntt());
	}

	void EntityRegistry::DestroyBehavior(Entity entity, const Behavior* beh) {
		behaviorsToDestroy.push_back(std::make_pair(entity.__GetEntt(), beh));
	}

}