#ifndef _CORE_GAME_CONTEXT_HPP
#define _CORE_GAME_CONTEXT_HPP
#include "../General/Macros.hpp"
#include "Level.hpp"

class GameContext {
public:

	static class Game* game;
	static LevelManager* levelManager;

	static Level* GetLevel() {
		if (levelManager)
			return levelManager->GetLevel();
		else return nullptr;
	}

	NON_CONSTRUCTABLE(GameContext)
};


#endif // !_CORE_GAME_CONTEXT_HPP