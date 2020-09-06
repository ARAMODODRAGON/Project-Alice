#ifndef ALICE_GAME_HPP
#define ALICE_GAME_HPP
#include <Engine/Game.hpp>

class AliceGame : public Game {

	int quitTimer;
	const int max_quit_time;

	// objects
	ObjectIndex* entityIndex;
	ObjectIndex* scriptableIndex;

	// level manager
	LevelManager* levelManager;

	// scriptable objects
	ScriptableObjectFactory* soFactory;

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