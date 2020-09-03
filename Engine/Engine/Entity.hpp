#ifndef _ENTITY_HPP
#define _ENTITY_HPP
#include "GeneralTypes.hpp"
#include "Physics/Overlap.hpp"
#include "RTTR.hpp"

class Entity {
	// base properties
	bool isActive;
	string name;
	vec2 position;

	// sprite properties
	bool isVisible;
	unsigned int spriteIndex; // uses the tiling to determine what part of texture to draw. default 0
	vec2 pivot; // the center of the quad. in pixel coords default { 0, 0 } (top-left)
	vec2 scale; // scales the quad. default { 1, 1 }
	float rotation; // rotation in degrees around the pivot
	int layer; // the layer to draw on (-100 to +100)
	/* Texture texture; */ // texture used for drawing. loaded from file
	/* Shader shader; */ // shader used for drawing. loaded from file
	array<vec2, 4> verticies; // quad being drawn to
	array<uvec3, 2> indicies; // used to draw quad
	vec2 spriteTiling; // the size of one tiling for drawing sprites. in pixel coords. loaded from file
	/* void Draw(const Camera* ca) { } */

	// physics properties
	vector<Overlap*> overlaps; // tracking collisions so the appropriate function can be called
	bool isSimulated; // should this entity be running physics?
	bool isAnchored; // should this entity act like a wall on collision?
	vec2 velocity;
	float colliderRadius;
	bitset<32> collisionMask;
	bitset<32> overlapMask;

	// events
	virtual void Start() { }
	virtual void Update() { }
	virtual void LateUpdate() { }
	virtual void OnDestroy() { }

	// static functions
	static void Destroy(const Entity* e) { }
	template<class T> static T* CreateEntity() { }

	// constructor & destructor
	Entity();
	virtual ~Entity();

	RTTR_ENABLE() RTTR_REGISTRATION_FRIEND
};

#endif // !_ENTITY_HPP