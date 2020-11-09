#ifndef SPELL_LIST_HPP
#define SPELL_LIST_HPP

#include "../../Common.hpp"
#include <fstream>

#define	NAME	"Name"
#define INFO	"Info"

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

using namespace std;
class SpellList {
	NON_CONSTRUCTABLE(SpellList);
public:
	static void InitSpellData(string _filePath);
	static Spell GetSpell(string _spellName);
	static uint32 GetTotalSpells();
};

#endif