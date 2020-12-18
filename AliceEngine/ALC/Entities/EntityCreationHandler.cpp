#include "EntityCreationHandler.hpp"

namespace ALC {

	void EntityCreatorComponent::Create(function<void(Entity)> callable) { 
		m_createCallables.push_back(callable);
	}

	vector<function<void(Entity)>>& ALC::EntityCreatorComponent::__GetCallables() {
		return m_createCallables;
	}


	EntityCreationHandler::EntityCreationHandler() { 
		m_createCallables.reserve(100);
	}
	EntityCreationHandler::~EntityCreationHandler() { }

	void EntityCreationHandler::CreateEntities(Registry& registry) { 
		// grab all of the functions
		registry.ForeachComponent<EntityCreatorComponent>(
			[this](Entity e, EntityCreatorComponent& ecc){
			auto& c = ecc.__GetCallables();
			for (size_t i = 0; i < c.size(); i++) {
				m_createCallables.push_back(c[i]);
			}
			c.clear();
		});

		// create each entity
		vector<entt::entity> entities;
		entities.resize(m_createCallables.size());

		registry.__GetReg().create(entities.begin(), entities.end());

		// call on each
		for (size_t i = 0; i < entities.size(); i++) {
			Entity e(entities[i], &registry);
			m_createCallables[i](e);
		}

		// clear 
		m_createCallables.clear();
	}

}