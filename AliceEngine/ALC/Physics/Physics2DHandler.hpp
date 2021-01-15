#ifndef ALC_PHYSICS_PHYSICS2DHANDLER_HPP
#define ALC_PHYSICS_PHYSICS2DHANDLER_HPP
#include "../General.hpp"
#include "../Entities/Registry.hpp"
#include "Rigidbody2D.hpp"

namespace ALC {

	class Physics2DHandler final {
		ALC_NO_COPY(Physics2DHandler);
	public:

		Physics2DHandler();
		~Physics2DHandler();

		// updates the physics
		void Step(Registry& registry, Timestep t);

	private:

		bool DoTrigger(Transform2D& t0, Rigidbody2D& r0,
					   Transform2D& t1, Rigidbody2D& r1);

		void UpdateCollisionInfo(EntityInfo& i0, Rigidbody2D& r0,
								 EntityInfo& i1, Rigidbody2D& r1);
	};

}

#endif // !ALC_PHYSICS_PHYSICS2DHANDLER_HPP