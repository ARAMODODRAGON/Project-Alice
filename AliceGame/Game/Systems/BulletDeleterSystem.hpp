#ifndef SYSTEMS_BULLETDELETERSYSTEM_HPP
#define SYSTEMS_BULLETDELETERSYSTEM_HPP
#include <ALC\Entities\Registry.hpp>
#include <ALC\Entities\EntityCreationHandler.hpp>
#include <ALC\Bullets\BulletPhysics.hpp>
#include <ALC\Rendering\SpriteBatch.hpp>

struct BulletDeleterComponent final {

	// if [lifetime > 0] then it will countdown to its death
	// if [lifetime <= 0] then it will only die when outside the bounds specified in the BulletDeleterSystem
	float lifetime = 0.0f;

	// deletes the bullet if it hits something
	bool deleteOnContact = true;

};

struct BulletDeleterSystem final : public ALC::ISystem<BulletDeleterComponent, ALC::BulletBody> {

	BulletDeleterSystem(ALC::EntityCreationHandler& ech);

	// setter
	void SetDeathBoundry(const ALC::rect& deathBoundry);

private:

	void Step(ALC::Timestep ts, ALC::Entity e, BulletDeleterComponent& bdc, ALC::BulletBody& bb) override;

	ALC::EntityCreationHandler& m_ech;
	ALC::rect m_deathBoundry;
};

#endif // !SYSTEMS_BULLETDELETERSYSTEM_HPP