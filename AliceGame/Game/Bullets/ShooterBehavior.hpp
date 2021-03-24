#ifndef ALC_BULLETS_SHOOTERBEHAVIOR_HPP
#define ALC_BULLETS_SHOOTERBEHAVIOR_HPP
#include <ALC/Entities/Registry.hpp>
#include "BulletPhysics.hpp"
#include <ALC/Entities/EntityCreationHandler.hpp>
#include <ALC/Rendering/SpriteBatch.hpp>

// used to specify the types to use when fireing bullets
template<typename... Components>
struct BulletTypes { };

// class used to contextualize the shooting behavior of 
// characters in the game
// requires the EntityCreationHandler to be used
class ShooterBehavior : public ALC::Behavior {
public:

	ShooterBehavior() : m_defaults{} { }
	virtual ~ShooterBehavior() = 0 { }

	// sets the default velocity of the bullet
	void SetDefaultVelocity(const ALC::vec2& defaultVelocity);

	// sets the default position of the bullet
	void SetDefaultPosition(const ALC::vec2& defaultPosition);

	// sets the default layermask for the bullet
	void SetDefaultCollisionmask(const ALC::Layermask32& defaultLayermask);

	// sets the default sprite layer
	void SetDefaultSpriteLayer(const ALC::int32 layer);

	// shoots n number of bullets
	template<typename Callable, typename... Components>
	void Shoot(ALC::Entity self, const ALC::uint32 n, Callable callable, BulletTypes<Components...> = BulletTypes<>());

	// shoots n number of bullets with angle offset
	template<typename Callable, typename... Components>
	void Shoot(ALC::Entity self, const ALC::uint32 n, const float angleInDegrees, Callable callable, BulletTypes<Components...> = BulletTypes<>());

	// shoots n number of bullets in a circle
	template<typename Callable, typename... Components>
	void ShootCircle(ALC::Entity self, const ALC::uint32 n, Callable callable, BulletTypes<Components...> = BulletTypes<>());

	// shoots n number of bullets in a range
	template<typename Callable, typename... Components>
	void ShootRange(ALC::Entity self, const ALC::uint32 n, const float rangeInDegrees, Callable callable, BulletTypes<Components...> = BulletTypes<>());

private:

	struct Default {
		ALC::vec2 velocity;
		ALC::vec2 position;
		ALC::int32 sprlayer;
		ALC::Layermask32 collisionmask;
	} m_defaults;

	void InitBullet(Default def, ALC::Transform2D& tr, BulletBody& bb, ALC::SpriteComponent& spr);

};



#include "detail\ShooterBehavior.inl"

#endif // !ALC_BULLETS_SHOOTERBEHAVIOR_HPP