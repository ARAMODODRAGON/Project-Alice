#ifndef ALC_ENTITIES_ENTITYCREATIONHANDLER_HPP
#define ALC_ENTITIES_ENTITYCREATIONHANDLER_HPP
#include "Registry.hpp"

namespace ALC {

	struct EntityCreatorComponent final {

		// prepares creation code for an entity to be invoked later
		void Create(function<void(Entity)> callable);

		// marks an entity as destructable
		void Destroy(Entity e);

	private:
		vector<function<void(Entity)>> m_createCallables;
		vector<entt::entity> m_destructables;
	public:
		vector<function<void(Entity)>>& __GetCallables();
		vector<entt::entity>& __GetDestructables();
	};

	class EntityCreationHandler final {
	public:

		EntityCreationHandler();
		~EntityCreationHandler();

		// creates and destroys 
		void Cleanup(Registry& registry);

		// prepares creation code for an entity to be invoked later
		void Create(function<void(Entity)> callable);

		// mark entity as destructable
		void Destroy(Entity e);

	private:
		vector<function<void(Entity)>> m_createCallables;
		vector<entt::entity> m_destructables;
	};

}

#include "detail\EntityCreationHandler.inl"

#endif // !ALC_ENTITIES_ENTITYCREATIONHANDLER_HPP