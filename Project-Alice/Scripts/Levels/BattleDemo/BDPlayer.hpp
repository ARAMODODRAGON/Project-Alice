#ifndef BATTLE_DEMO_PLAYER_HPP
#define BATTLE_DEMO_PLAYER_HPP
#include <Engine\Battle\BTPlayer.hpp>
#include <Engine\Battle\ShootComponent.hpp>

class BDPlayer : public BTPlayer {
public:

	BDPlayer();
	~BDPlayer() final;

	void Start() override;
	void Update() override;
	void LateUpdate() override;

private:
	ShootComponent* shcp;
};

#endif // !BATTLE_DEMO_PLAYER_HPP