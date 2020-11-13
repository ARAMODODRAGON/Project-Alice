#ifndef SPELL_INVENTORY_HPP
#define SPELL_INVENTORY_HPP

#include <Engine/Battle/Spells/SpellList.hpp>
#include <Engine/General/SaveSystem.hpp>

class SpellInventory : public ISaveable {
	PRIVATE_SINGLETON(SpellInventory);
public:
	SpellInventory() {};
	~SpellInventory() {};
	static void InitData();

	static void AddSpell(const string& _spellName);
	static void RemoveSpell(const string& _spellName);

	static void EquipAtkSpell(const string& _spellName, int _index);
	static void EquipDefSpell(const string& _spellName);
	static void UnequipAtkSpell(int _index);
	static void UnequipDefSpell();

	static Spell GetSpell(const string& _spellName);
	static int GetSpellCount(SpellType _type);

	static array<string, MAX_EQUIPPED_SPELLS> GetEquippedAtkSpells();
	static string GetEquippedAtkSpell(int _index);
	static string GetEquippedDefSpell();

	void SaveData(json& _data) override;
	void LoadData(json& _data) override;
};

#endif