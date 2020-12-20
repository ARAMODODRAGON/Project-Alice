#ifndef ALC_BULLETS_BULLETPHYSICSHANDLER_HPP
#define ALC_BULLETS_BULLETPHYSICSHANDLER_HPP
#include "BulletPhysics.hpp"
#include "../Entities/Registry.hpp"

namespace ALC {

	class BulletPhysicsHandler {
	public:

		BulletPhysicsHandler();
		~BulletPhysicsHandler();

		void Step(Registry& registry, Timestep t);


	private:
	};

}

#endif // !ALC_BULLETS_BULLETPHYSICSHANDLER_HPP