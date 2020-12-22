#ifndef SYSTEMS_BULLETDELETERSYSTEM_HPP
#define SYSTEMS_BULLETDELETERSYSTEM_HPP
#include <ALC\Entities\Registry.hpp>
#include <ALC\Entities\EntityCreationHandler.hpp>
#include <ALC\Bullets\BulletPhysics.hpp>
#include <ALC\Rendering\SpriteBatch.hpp>

struct BulletDeleterComponent final {

	// if lifetime > 0 then itll countdown to its death
	// if lifetime <= 0 then itll only die when outside the bounds specified in the BulletDeleterSystem
	float lifetime = 0.0f;

};

struct BulletDeleterSystem final : public ALC::ISystem<BulletDeleterComponent> {

	BulletDeleterSystem(ALC::EntityCreationHandler& ech);

	// setter
	void SetDeathBoundry(const ALC::rect& deathBoundry);

private:

	void Step(ALC::Timestep ts, ALC::Entity e, BulletDeleterComponent& bdc) override;

	ALC::EntityCreationHandler& m_ech;
	ALC::rect m_deathBoundry;
};

#endif // !SYSTEMS_BULLETDELETERSYSTEM_HPP