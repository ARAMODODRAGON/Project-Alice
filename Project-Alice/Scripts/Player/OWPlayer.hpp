#ifndef PLAYER_OWPLAYER_HPP
#define PLAYER_OWPLAYER_HPP
#include <Engine/Objects/Entity.hpp>

class OWPlayer : public Entity {

	array<unsigned int, 4> inventory;

public:

	OWPlayer();
	~OWPlayer();

	// events
	virtual void Start();
	virtual void Update();
	virtual void LateUpdate();
	virtual void OnDestroy();

	RTTR_ENABLE(Entity) RTTR_REGISTRATION_FRIEND
};

#endif // !PLAYER_OWPLAYER_HPP