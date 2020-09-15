#ifndef PLAYER_OWPLAYER_HPP
#define PLAYER_OWPLAYER_HPP
#include <Engine/Common.hpp>

class OWPlayer : public Object {

	Sprite* sprite;
	float colorMix;
	bool increaseMix;

public:

	OWPlayer();
	~OWPlayer();

	void Start() override;
	void Update() override;

	RTTR_ENABLE(Object)
};

#endif // !PLAYER_OWPLAYER_HPP