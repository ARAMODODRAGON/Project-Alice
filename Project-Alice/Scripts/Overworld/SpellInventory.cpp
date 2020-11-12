#include "SpellInventory.hpp"
#include <fstream>

static list<string> curSpells = list<string>();
static array<string, MAX_EQUIPPED_SPELLS> curAtkSpells;
static string curDefSpell;

void SpellInventory::InitData(const string& _filePath) {
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
	// Loading in the player's full inventory
	if (file.contains(INVENTORY) && file[INVENTORY].is_array()) {
		json& inventory = file[INVENTORY];
		for (uint32 i = 0; i < inventory.size(); i++) {
			AddSpell(inventory[i]); // Add in each spell to the inventory
		}
	}
	// Loading in the player's equipped attack spells
	if (file.contains(EQUIPPED_ATTACKS) && file[EQUIPPED_ATTACKS].is_array()) {
		json& atkSpells = file[EQUIPPED_ATTACKS];
		for (uint32 i = 0; i < atkSpells.size(); i++) {
			EquipAtkSpell(atkSpells[i], i);
		}
	}
	// Loading in the player's equipped defence spell
	if (file.contains(EQUIPPED_DEFENCE)) {
		json& defSpell = file[EQUIPPED_DEFENCE];
		EquipDefSpell(defSpell);
	}
}

void SpellInventory::AddSpell(const string& _spellName) {
	for (auto it = curSpells.begin(); it != curSpells.end(); it++) {
		if (*it == _spellName) {
			DEBUG_LOG("Requested skill is already in the player's inventory.");
			return; // Don't try adding in the same spell twice
		}
	}
	curSpells.push_back(_spellName);
}

void SpellInventory::RemoveSpell(const string& _spellName) {
	curSpells.remove(_spellName);
}

void SpellInventory::EquipAtkSpell(const string& _spellName, int _index) {
	// First, make sure the spell is actually an attack spell, exit if it isn't one
	if (SpellList::GetSpell(_spellName).type != SpellType::Attack) {
		DEBUG_WARNING("Spell must be a attack spell. No spell was equipped.");
		return;
	}
	// Next, check to see if this skill has already been equipped, exit if it is already equipped
	for (uint32 i = 0; i < MAX_EQUIPPED_SPELLS; i++) {
		if (curAtkSpells[i] == _spellName) {
			DEBUG_WARNING("Spell is already equipped!");
			return;
		}
	}
	// Only equip the skill if the index is within the valid range
	if (_index >= 0 && _index < MAX_EQUIPPED_SPELLS) {
		curAtkSpells[_index] = _spellName;
		return;
	}
	DEBUG_WARNING("Index provided was outside of the valid range. No skill was equipped.");
}

void SpellInventory::EquipDefSpell(const string& _spellName) {
	// First, make sure the spell is actually a defence spell, exit if it isn't one
	if (SpellList::GetSpell(_spellName).type != SpellType::Defence) {
		DEBUG_WARNING("Spell must be a defence spell. No spell was equipped.");
		return;
	}
	// Next, make sure the defence skill isn't already equipped, exit it is already is equipped
	if (curDefSpell == _spellName) {
		DEBUG_WARNING("That defence skill is already equipped");
		return;
	}
	// If both checks are passed, equip the spell
	curDefSpell = _spellName;
}

void SpellInventory::UnequipAtkSpell(int _index) {
	if (_index >= 0 && _index < MAX_EQUIPPED_SPELLS) {
		curAtkSpells[_index] = "";
	}
}

void SpellInventory::UnequipDefSpell() {
	curDefSpell = "";
}

Spell SpellInventory::GetSpell(const string&_spellName) {
	for (auto it = curSpells.begin(); it != curSpells.end(); it++) {
		if (*it == _spellName) {
			return SpellList::GetSpell(_spellName);
		}
	}
	return Spell{}; // Return an empty struct if the spell doesn't exist
}

int SpellInventory::GetSpellCount(SpellType _type) {
	int totalSpells = 0;
	for (auto it = curSpells.begin(); it != curSpells.end(); it++) {
		if (SpellList::GetSpell(*it).type == _type) {
			totalSpells++;
		}
	}
	return totalSpells;
}

array<string, MAX_EQUIPPED_SPELLS> SpellInventory::GetEquippedAtkSpells() {
	return curAtkSpells;
}

string SpellInventory::GetEquippedAtkSpell(int _index) {
	if (_index >= 0 && _index < MAX_EQUIPPED_SPELLS) {
		return curAtkSpells[_index];
	}
	return "ERROR";
}

string SpellInventory::GetEquippedDefSpell() {
	return curDefSpell;
}