#ifndef BATTLE_DEMO_PLAYER_HPP
#define BATTLE_DEMO_PLAYER_HPP
#include <Engine\Battle\BTPlayer.hpp>

class BDPlayer : public BTPlayer {
public:

	BDPlayer();
	~BDPlayer() final;

	void Start() override;
	void Update() override;
	void LateUpdate() override;

private:

};

#endif // !BATTLE_DEMO_PLAYER_HPP