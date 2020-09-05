#ifndef _CORE_LEVEL_HPP
#define _CORE_LEVEL_HPP
#include "../Objects/EntityFactory.hpp"
#include "../General/Serialization.hpp"

class LevelManager;

class Level {

	// levelmanager that this is attached to
	LevelManager* levelManager;

	EntityFactory* eFactory;

public:

	Level(const json& data, LevelManager* levelManager_, ObjectIndex* entityIndex_ = nullptr);
	~Level();

	// events
	void Update();
	void Draw();

};

#endif // !_CORE_LEVEL_HPP