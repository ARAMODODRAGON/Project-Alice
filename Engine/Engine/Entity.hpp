#ifndef _ENTITY_HPP
#define _ENTITY_HPP
#include "General/Types.hpp"
#include "Physics/Overlap.hpp"
#include "General/Serialization.hpp"

class Entity {
	// base properties
	class Level* level;
	bool isActive;
	string name;
	vec2 position;
	vec2 velocity;

	// sprite properties
	bool isVisible;
	unsigned int spriteIndex;
	/* Texture texture; */
	/* Shader shader; */
	array<vec2, 4> verticies;
	array<uvec3, 2> indicies;
	unsigned int VAO, VBO, EBO;
	vec2 spriteTiling; // the size of one tiling for drawing sprites. in pixel coords. loaded from file
	vec2 pivot;
	vec2 scale;
	float rotation; 
	int layer;
	/* void Draw(const Camera* ca) { } */

	///////////////////// Move to collider class ////////////////////////////
	// physics properties
	vector<Overlap*> overlaps; // tracking collisions so the appropriate function can be called
	bool isSimulated; // should this entity be running physics?
	bool isAnchored; // should this entity act like a wall on collision?

	float colliderRadius;
	bitset<32> collisionMask;
	bitset<32> overlapMask;
	/////////////////////////////////////////////////////////////////////////

public:

	// constructor & destructor
	Entity();
	virtual ~Entity() = 0; // abstract class

	// events
	virtual void Start() { }
	virtual void Update() { }
	virtual void LateUpdate() { }
	virtual void OnDestroy() { }

	// static functions
	static void Destroy(const Entity* e) { }
	template<class T> static T* CreateEntity() { }

	RTTR_ENABLE() RTTR_REGISTRATION_FRIEND
};

#endif // !_ENTITY_HPP