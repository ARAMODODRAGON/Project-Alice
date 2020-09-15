#ifndef PLAYER_OWPLAYER_HPP
#define PLAYER_OWPLAYER_HPP
#include <Engine/Common.hpp>

class OWPlayer : public Object {

	Sprite* sprite;
	Camera* cam;
	float colorMix;
	bool increaseMix;
	unsigned int animTimer;
	bool facingUp : 1;

public:

	OWPlayer();
	~OWPlayer();

	void Start() override;
	void Update() override;
	void LateUpdate() override;

	RTTR_ENABLE(Object)
};

#endif // !PLAYER_OWPLAYER_HPP