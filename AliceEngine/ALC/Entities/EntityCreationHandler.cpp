#include "EntityCreationHandler.hpp"

namespace ALC {

	void EntityCreatorComponent::Create(function<void(Entity)> callable) {
		m_createCallables.push_back(callable);
	}

	void EntityCreatorComponent::Destroy(Entity e) {
		m_destructables.push_back(e.__GetEntt());
	}

	vector<function<void(Entity)>>& EntityCreatorComponent::__GetCallables() {
		return m_createCallables;
	}

	vector<entt::entity>& EntityCreatorComponent::__GetDestructables() {
		return m_destructables;
	}


	EntityCreationHandler::EntityCreationHandler() {
		m_createCallables.reserve(100);
		m_destructables.reserve(100);
	}
	EntityCreationHandler::~EntityCreationHandler() { }

	void EntityCreationHandler::Cleanup(Registry& registry) {
		// grab all of the functions and destructables
		registry.ForeachComponent<EntityCreatorComponent>(
			[this](Entity e, EntityCreatorComponent& ecc) {
			auto& c = ecc.__GetCallables();
			auto& d = ecc.__GetDestructables();
			for (size_t i = 0; i < c.size(); i++) {
				m_createCallables.push_back(c[i]);
			}
			for (size_t i = 0; i < d.size(); i++) {
				m_destructables.push_back(d[i]);
			}
			c.clear();
			d.clear();
		});

		// get registry
		auto& reg = registry.__GetReg();

		// destroy each
		reg.destroy(m_destructables.begin(), m_destructables.end());

		// create each entity
		vector<entt::entity> entities;
		entities.resize(m_createCallables.size());

		reg.create(entities.begin(), entities.end());

		// call on each
		for (size_t i = 0; i < entities.size(); i++) {
			Entity e(entities[i], &registry);
			m_createCallables[i](e);
		}

		// clear 
		m_createCallables.clear();
		m_destructables.clear();
	}

	void EntityCreationHandler::Create(function<void(Entity)> callable) {
		m_createCallables.push_back(callable);
	}

	void EntityCreationHandler::Destroy(Entity e) {
		m_destructables.push_back(e.__GetEntt());
	}

}