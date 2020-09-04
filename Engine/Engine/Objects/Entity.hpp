#ifndef _ENTITY_HPP
#define _ENTITY_HPP
#include "../General/Types.hpp"
#include "../Physics/Overlap.hpp"
#include "../General/Serialization.hpp"
#include "EntityFactory.hpp"

class Entity {
	// factory
	friend EntityFactory;
	EntityFactory* factory; // the factory that this entity is attached to

	// base properties
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
	virtual ~Entity(); // abstract class

	// events
	virtual void Start() { }
	virtual void Update() { }
	virtual void LateUpdate() { }
	virtual void OnDestroy() { }

	// static functions
	//static void Destroy(const Entity* e) { }
	//template<class T> static T* CreateEntity() { }

	// getters & setters
	const string& GetName() const { return name; }
	void SetName(const string& name_) { name = name_; }
	const vec2& GetVelocity() const { return velocity; }
	void SetVelocity(const vec2& velocity_) { velocity = velocity_; }
	const vec2& GetPosition() const { return position; }
	void SetPosition(const vec2& position_) { position = position_; }

	RTTR_ENABLE() RTTR_REGISTRATION_FRIEND
};

#endif // !_ENTITY_HPP