#ifndef _PHYSICS_OVERLAP_HPP
#define _PHYSICS_OVERLAP_HPP

class Entity;

struct Overlap {
	enum : char {
		EntityType,
		TilemapType,
		BulletType
	} type;
	union {
		Entity* otherEntity; // the other entity of this overlap. null if it was not an entiity
		/* Tilemap* tilemap; */ // the tilemap in this collision. null if it was not a tilemap
		/* Bullet* bullet; */ // the bullet involved in this collision. null if it was not a bullet
	};
};

#endif // !_PHYSICS_OVERLAP_HPP