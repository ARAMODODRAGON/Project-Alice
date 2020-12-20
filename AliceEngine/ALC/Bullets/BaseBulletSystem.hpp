#ifndef ALC_BULLETS_BASEBULLETSYSTEM_HPP
#define ALC_BULLETS_BASEBULLETSYSTEM_HPP
#include "../Entities/Registry.hpp"
#include "BulletComponent.hpp"
#include "BulletPhysics.hpp"
#include "../Entities/EntityCreationHandler.hpp"

namespace ALC {

	// system that applies regular bullet mechanics
	struct BaseBulletSystem final : public ISystem<BulletComponent, BulletBody, Transform2D> {

		EntityCreationHandler& ech;
		vec2 bounds;

		BaseBulletSystem(EntityCreationHandler& ech_) ;

		void Step(Timestep ts, const Entity, BulletComponent&, BulletBody&, Transform2D&) override;
	};

}

#endif // !ALC_BULLETS_BASEBULLETSYSTEM_HPP