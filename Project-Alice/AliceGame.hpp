#ifndef ALICE_GAME_HPP
#define ALICE_GAME_HPP
#include <Engine/Game.hpp>
#include <Engine/Objects/ObjectIndex.hpp>
#include <Engine/Objects/Entity.hpp>

class AliceGame : public Game {

	int quitTimer;
	const int max_quit_time;

	// objects
	ObjectIndex* entityIndex;
	ObjectIndex* levelIndex;
	ObjectIndex* scriptableIndex;

	// factory
	EntityFactory* efactory;

public:

	AliceGame();
	~AliceGame();

private:

	// events
	bool Start() override;
	void Update() override;
	void Draw() override;
	bool Exit() override;

};

#endif // !ALICE_GAME_HPP