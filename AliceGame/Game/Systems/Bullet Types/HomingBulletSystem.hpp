#ifndef ALC_SYSTEMS_BULLETTYPES_HOMINGBULLETSYSTEM_HPP
#define ALC_SYSTEMS_BULLETTYPES_HOMINGBULLETSYSTEM_HPP
#include <ALC\Entities.hpp>
#include <ALC\Bullets.hpp>
#include <ALC\Rendering.hpp>

struct HomingBullet {

	// the rotation speed of the bullet
	// [requires >= 0]
	float rotationSpeed = 40.0f;

	// the rotation acceleration per second
	// [requires >= 0]
	float rotationAccel = 150.0f;

	// the target is decided automatically by the system
	ALC::uint32 GetTargetID() const { return __targetID; }

public:
	ALC::uint32 __targetID = ALC::uint32(-1);
};

struct HomingBulletSystem : public ALC::ISystem<ALC::BulletBody, ALC::Transform2D, HomingBullet> {

	HomingBulletSystem(ALC::Registry& reg);
	~HomingBulletSystem();

	void Step(ALC::Timestep ts, ALC::Entity e, ALC::BulletBody& bb, ALC::Transform2D& tr, HomingBullet& homing) override;

	void AddTarget(ALC::EntityID target);
	void RemoveTarget(ALC::EntityID target);

private:

	ALC::Registry& m_reg;
	ALC::vector<ALC::EntityID> m_targets;

};

#endif // !ALC_SYSTEMS_BULLETTYPES_HOMINGBULLETSYSTEM_HPP