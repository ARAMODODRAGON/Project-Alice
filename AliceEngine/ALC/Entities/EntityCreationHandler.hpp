#ifndef ALC_ENTITIES_ENTITYCREATIONHANDLER_HPP
#define ALC_ENTITIES_ENTITYCREATIONHANDLER_HPP
#include "Registry.hpp"

namespace ALC {

	struct EntityCreatorComponent final {

		// prepares creation code for an entity to be invoked later
		void Create(function<void(Entity)> callable);

	private:
		vector<function<void(Entity)>> m_createCallables;
	public:
		vector<function<void(Entity)>>& __GetCallables();
	};

	class EntityCreationHandler final {
	public:

		EntityCreationHandler();
		~EntityCreationHandler();

		void CreateEntities(Registry& registry);

	private:
		vector<function<void(Entity)>> m_createCallables;
	};

}

#include "detail\EntityCreationHandler.inl"

#endif // !ALC_ENTITIES_ENTITYCREATIONHANDLER_HPP