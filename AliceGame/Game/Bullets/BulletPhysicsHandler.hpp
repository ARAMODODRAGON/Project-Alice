#ifndef ALC_BULLETS_BULLETPHYSICSHANDLER_HPP
#define ALC_BULLETS_BULLETPHYSICSHANDLER_HPP
#include "BulletPhysics.hpp"
#include <ALC/Entities/Registry.hpp>

class BulletPhysicsHandler {
public:

	BulletPhysicsHandler();
	~BulletPhysicsHandler();

	void Step(ALC::Registry& registry, ALC::Timestep t);


private:
};


#endif // !ALC_BULLETS_BULLETPHYSICSHANDLER_HPP