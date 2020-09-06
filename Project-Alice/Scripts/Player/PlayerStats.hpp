#ifndef PLAYER_PLAYER_STATS_HPP
#define PLAYER_PLAYER_STATS_HPP
#include <Engine/Objects/ScriptableObject.hpp>

class PlayerStats : public ScriptableObject {
public:

	PlayerStats();
	~PlayerStats();

	void Start() override;

	RTTR_ENABLE(ScriptableObject) RTTR_REGISTRATION_FRIEND
};

#endif // !PLAYER_PLAYER_STATS_HPP