#ifndef BATTLE_SPELL_HPP
#define BATTLE_SPELL_HPP

#include "SpellMacros.hpp"
#include "../BTPlayer.hpp"

class BTSpell {
	BTPlayer* player;

public:
	BTSpell() : player(nullptr) {};
	virtual ~BTSpell() {};

	BTPlayer* GetPlayer() { return player; }
	void SetPlayer(BTPlayer* _player) { player = _player; }

private:
	virtual void Shoot() {};

};

#endif