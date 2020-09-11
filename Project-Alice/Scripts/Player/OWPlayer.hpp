#ifndef PLAYER_OWPLAYER_HPP
#define PLAYER_OWPLAYER_HPP
#include <Engine/Common.hpp>

class OWPlayer : public Object {

public:

	OWPlayer();
	~OWPlayer();

	void Start() override;

	RTTR_ENABLE(Object)
};

#endif // !PLAYER_OWPLAYER_HPP