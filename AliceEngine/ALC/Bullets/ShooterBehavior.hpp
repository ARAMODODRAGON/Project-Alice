#ifndef ALC_BULLETS_SHOOTERBEHAVIOR_HPP
#define ALC_BULLETS_SHOOTERBEHAVIOR_HPP
#include "../Entities/Registry.hpp"
#include "BulletPhysics.hpp"
#include "../Entities/EntityCreationHandler.hpp"
#include "../Rendering/SpriteBatch.hpp"

namespace ALC {

	// used to specify the types to use when fireing bullets
	template<typename... Components>
	struct BulletTypes {};

	// class used to contextualize the shooting behavior of 
	// characters in the game
	// requires the EntityCreationHandler to be used
	class ShooterBehavior : public Behavior {
	public:

		ShooterBehavior() : m_defaultVelocity(0.0f) { }
		virtual ~ShooterBehavior() = 0 { }

		// sets the default velocity of the bullet
		void SetDefaultVelocity(const vec2& defaultVelocity);

		// sets the default position of the bullet
		void SetDefaultPosition(const vec2& defaultPosition);

		// sets the default layermask for the bullet
		void SetDefaultCollisionmask(const Layermask32& defaultLayermask);

		// sets the default sprite layer
		void SetDefaultSpriteLayer(const int32 layer);

		// shoots n number of bullets
		template<typename Callable, typename... Components>
		void Shoot(Entity self, const uint32 n, Callable callable, BulletTypes<Components...> = BulletTypes<>());

		// shoots n number of bullets with angle offset
		template<typename Callable, typename... Components>
		void Shoot(Entity self, const uint32 n, const float angleInDegrees, Callable callable, BulletTypes<Components...> = BulletTypes<>());

		// shoots n number of bullets in a circle
		template<typename Callable, typename... Components>
		void ShootCircle(Entity self, const uint32 n, Callable callable, BulletTypes<Components...> = BulletTypes<>());

		// shoots n number of bullets in a range
		template<typename Callable, typename... Components>
		void ShootRange(Entity self, const uint32 n, const float rangeInDegrees, Callable callable, BulletTypes<Components...> = BulletTypes<>());

	private:

		void InitBullet(Transform2D& tr, BulletBody& bb, SpriteComponent& spr);

		vec2 m_defaultVelocity;
		vec2 m_defaultPosition;
		int32 m_defaultSprlayer;
		Layermask32 m_defaultCollisionmask;
	};

}

#include "detail\ShooterBehavior.inl"

#endif // !ALC_BULLETS_SHOOTERBEHAVIOR_HPP