#ifndef ALC_BULLETS_BULLETPHYSICS_HPP
#define ALC_BULLETS_BULLETPHYSICS_HPP
#include "../General.hpp"
#include "../Layermask.hpp"
#include "../Physics/CollisionInfo.hpp"
#include "../Physics/Transform2D.hpp"

namespace ALC {

	struct BulletBody final {

		// is physics applied
		bool isSimulated = true;

		// velocity of the bullet
		vec2 velocity = vec2(0.0f);

		// radius of the bullet
		// physics does not apply if the radius is < 0.0001
		float radius = 0.5f;

		// what layers does this bullet collide with
		Layermask32 mask = Layermask32::ALL;

	};

	struct CharacterBody final {

		// is physics applied
		bool isSimulated = true;

		// velocity of the character
		vec2 velocity = vec2(0.0f);

		// radius of the character
		// physics does not apply if the radius is < 0.0001
		float radius = 0.5f;

		// what layers does this character collide with
		Layermask32 mask = Layermask32::ALL;

		// iterator used for iterating the collision list
		using const_iterator = vector<CollisionInfo>::const_iterator;

		// iterator pointing to the begining of the collision list
		const_iterator begin() const { return m_collisions.begin(); }

		// iterator pointing to the end of the collision list
		const_iterator end() const { return m_collisions.end(); }

		// returns the number of collisions 
		size_t Count() const { return m_collisions.size(); }

		// returns collision at index
		const CollisionInfo& operator[](const size_t index) const { return m_collisions[index]; }

	private:
		vector<CollisionInfo> m_collisions;
	public:
		vector<CollisionInfo>& __GetColl() { return m_collisions; }
	};

}

#endif // !ALC_BULLETS_BULLETPHYSICS_HPP