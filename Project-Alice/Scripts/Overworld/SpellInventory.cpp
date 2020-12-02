#include "SpellInventory.hpp"

static list<string> curSpells = list<string>();
static array<string, MAX_EQUIPPED_SPELLS> curAtkSpells;
static string curDefSpell;

void SpellInventory::InitData() {
	SaveSystem::Register(INVENTORY, Get());
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

void SpellInventory::SaveData(json& _data) {
	_data[CURRENT_SPELLS] = curSpells;
	_data[EQUIPPED_ATTACKS] = curAtkSpells;
	_data[EQUIPPED_DEFENCE] = curDefSpell;
}

void SpellInventory::LoadData(json& _data) {
	if (_data.contains(INVENTORY) && _data[INVENTORY].is_object()) {
		json inventory = _data[INVENTORY];
		for (uint32 i = 0; i < inventory[CURRENT_SPELLS].size(); i++) {
			curSpells.push_back(inventory[CURRENT_SPELLS][i]);
		}
		curAtkSpells = inventory[EQUIPPED_ATTACKS];
		curDefSpell = inventory[EQUIPPED_DEFENCE];
	}
}