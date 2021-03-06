#include "SpellList.hpp"
#include <fstream>

static map<string, Spell> spells = map<string, Spell>();
static uint32 totalSpells = 0;

void SpellList::InitSpellData(const string& _filePath) {
	// Attempt to load in the JSON file's raw data
	ifstream stream;
	stream.open(_filePath);
	if (!stream.is_open()) {
		DEBUG_ERROR("Couldn't load file contained in path: " + _filePath);
		return;
	}
	json file;
	stream >> file;
	stream.close();
	// Loading in attack spell data
	if (file.contains(ATTACK_SPELLS) && file[ATTACK_SPELLS].is_array()) {
		json& attacks = file[ATTACK_SPELLS];
		for (uint32 i = 0; i < attacks.size(); i++) {
			json& spell = attacks[i];
			Spell spellData{};
			spellData.type = SpellType::Attack;
			spellData.name = spell[NAME];
			spellData.info = spell[INFO];
			spells.insert(std::make_pair(spell[NAME], spellData));
			totalSpells++;
		}
	}
	// Loading in defence spell data
	if (file.contains(DEFENCE_SPELLS) && file[DEFENCE_SPELLS].is_array()) {
		json& defences = file[DEFENCE_SPELLS];
		for (uint32 i = 0; i < defences.size(); i++) {
			json& spell = defences[i];
			Spell spellData{};
			spellData.type = SpellType::Defence;
			spellData.name = spell[NAME];
			spellData.info = spell[INFO];
			spells.insert(std::make_pair(spell[NAME], spellData));
			totalSpells++;
		}
	}
}

Spell SpellList::GetSpell(const string& _spellName) {
	return spells.at(_spellName);
}

uint32 SpellList::GetTotalSpells() {
	return totalSpells;
}