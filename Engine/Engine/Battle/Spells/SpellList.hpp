#ifndef SPELL_LIST_HPP
#define SPELL_LIST_HPP

#include "SpellMacros.hpp"
#include "../../Common.hpp"

enum SpellType {
	NoSpell,
	Attack,
	Defence
};

struct Spell {
	SpellType type;
	string name;
	string info;
};

class SpellList {
	NON_CONSTRUCTABLE(SpellList);
public:
	static void InitSpellData(const string& _filePath);
	static Spell GetSpell(const string& _spellName);
	static uint32 GetTotalSpells();
};

#endif