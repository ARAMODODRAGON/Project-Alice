#ifndef PLAYER_OWPLAYER_HPP
#define PLAYER_OWPLAYER_HPP
#include <Engine/Common.hpp>

class OWPlayer : public Object {

	enum class Facing : char {
		Right,
		Up,
		Down,
		Left
	};

	Sprite* sprite;
	Camera* cam;
	float colorMix;
	bool increaseMix;
	unsigned int animTimer;
	Facing facing;

	// projectile settings
	vec4 shootColor;
	float shootSpeed;
	unsigned int shootDestroyTimer;

public:

	OWPlayer();
	~OWPlayer();

	void Start() override;
	void Update() override;
	void LateUpdate() override;

	RTTR_ENABLE(Object) RTTR_REGISTRATION_FRIEND
};

#endif // !PLAYER_OWPLAYER_HPP