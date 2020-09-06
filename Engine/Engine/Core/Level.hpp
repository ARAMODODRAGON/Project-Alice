#ifndef _CORE_LEVEL_HPP
#define _CORE_LEVEL_HPP
#include "LevelManager.hpp"
#include "../Objects/EntityFactory.hpp"
#include "../General/Serialization.hpp"

class Level {

	// levelmanager that this is attached to
	LevelManager* levelManager;

	EntityFactory* eFactory;

	string updateText;
	string nextLevel;

public:

	Level(const json& data, LevelManager* levelManager_, ObjectIndex* entityIndex_ = nullptr);
	~Level();

	// events
	void Update();
	void Draw();

};

#endif // !_CORE_LEVEL_HPP