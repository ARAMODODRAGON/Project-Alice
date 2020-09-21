#ifndef ALICE_GAME_HPP
#define ALICE_GAME_HPP
#include <Engine/Game.hpp>
#include <cstdlib>
#include <time.h>

class AliceGame : public Game {

	int quitTimer;
	const int max_quit_time;


public:

	AliceGame();
	~AliceGame();

private:

	// events
	bool Init() override;
	void Update() override;
	void Draw() override;
	bool Exit() override;
	void LevelLoad(Level* level, const json& data) override;

};

#endif // !ALICE_GAME_HPP