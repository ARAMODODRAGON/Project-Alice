#ifndef SAVE_SYSTEM_HPP
#define SAVE_SYSTEM_HPP

#include "../Common.hpp"

// The Allocated Groups for the Player's Inventory Save Data
#define INVENTORY					"Inventory"
#define	CURRENT_SPELLS				"Current Spells"
#define EQUIPPED_ATTACKS			"Equipped Attacks"
#define	EQUIPPED_DEFENCE			"Equipped Defence"

class ISaveable {
public:
	ISaveable() {};
	virtual ~ISaveable() = 0 {};

	virtual void SaveData(json& _data) = 0;
	virtual void LoadData(json& _data) = 0;
};

using namespace std;
class SaveSystem {
	NON_CONSTRUCTABLE(SaveSystem);
public:
	static bool SaveData(const string& _fileName);
	static bool LoadData(const string& _fileName);

	static void Register(string _name, ISaveable* _ptr);
	static void Deregister(ISaveable* _ptr);
};

#endif