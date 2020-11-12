#ifndef SAVE_SYSTEM_HPP
#define SAVE_SYSTEM_HPP

#include "../Common.hpp"

class ISaveable {
public:
	ISaveable() {};
	virtual ~ISaveable() = 0 {};

	virtual void SaveData(json& _data) = 0;
	virtual void LoadData(json& _data) = 0;
};

class Test : public ISaveable {
	string data;
public:
	Test() { data = "This is a test"; };

	void SaveData(json& _data) override {
		_data.push_back(data);
	}
	void LoadData(json& _data) override {

	}
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