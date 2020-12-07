#ifndef NETWORKED_LEVEL_HPP
#define NETWORKED_LEVEL_HPP
#include "Engine\Core\Level.hpp"
#include "Engine\Battle\BTPlayer.hpp"
#include "../BattleDemo/BattleDemoCamera.hpp"
#include "Engine\Network\NetworkManager.hpp"

class NetPlayer;

class NetworkedLevel final : public Level {
public:

	NetworkedLevel() { }
	~NetworkedLevel() { }

	void Init() override;
	void Update() override;
	void LateUpdate() override;
	void Exit() override;

private:

	// counts frames
	uint32 counter;

	// the two players in this simulation
	NetPlayer* thisPlayer;
	NetPlayer* otherPlayer;
};

class NetPlayer final : public BTPlayer {
public:

	NetPlayer();
	~NetPlayer();

	void DoLocalMovement();
	void ReadInput(vec2 position, vec2 velocity);
	void Update() override { }
	void LateUpdate() override { }
};

//class ThisPlayer final : public NetPlayer {
//public:
//
//	ThisPlayer();
//	~ThisPlayer();
//
//	void Start() override;
//	void Update() override;
//	void LateUpdate() override;
//	void OnDestroy() override;
//
//private:
//
//};
//
//class OtherPlayer final : public NetPlayer {
//public:
//
//	OtherPlayer();
//	~OtherPlayer();
//
//	void Start() override;
//	void Update() override;
//	void LateUpdate() override;
//	void OnDestroy() override;
//
//private:
//
//};

#endif // !NETWORKED_LEVEL_HPP