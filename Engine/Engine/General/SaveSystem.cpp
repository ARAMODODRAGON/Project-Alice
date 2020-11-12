#include "SaveSystem.hpp"
#include <direct.h>
#include <fstream>

vector<pair<string, ISaveable*>> saveData = vector<pair<string, ISaveable*>>();

bool SaveSystem::SaveData(const string& _fileName) {
	json savefile;
	for (uint32 i = 0; i < saveData.size(); i++) {
		json data; // Data for each "ISaveable" instance will be stored in here and placed in the grater savefile JSON
		saveData[i].second->SaveData(data);
		savefile[saveData[i].first] = data;
	}
	// Before writing the data to the file, make a directory for the save data if it doesn't exist
	const char* appdata = getenv("APPDATA");
	string filePath(to_string(appdata) + "/Project-Alice");
	if (_mkdir(filePath.c_str()) != 0) {
		DEBUG_WARNING("There was a problem with creating the save directory OR it already exists!");
	}
	// Once the file path has been create if needed, write the file to that directory
	ofstream stream(filePath + "/" + _fileName + ".json");
	if (stream.is_open()) { // Make sure it's safe to begin writing data
		stream << savefile;
		stream.close();
		return true; // Save successful; return true
	}
	DEBUG_ERROR("Could not save data for an unknown reason...");
	return false; // Couldn't save for some reason; return false
}

bool SaveSystem::LoadData(const string& _fileName) {
	return false;
}

void SaveSystem::Register(string _name, ISaveable* _ptr) {
	for (uint32 i = 0; i < saveData.size(); i++) {
		if (saveData[i].first == _name || saveData[i].second == _ptr) { // If the pair being added matches an existing pair OR there's a duplicate pointer, don't add it to the data
			DEBUG_WARNING("This data has already been registered! It will not be registered again...");
			return;
		}
	}
	// Add the data into the save data vector
	saveData.push_back(std::make_pair(_name, _ptr));
}

void SaveSystem::Deregister(ISaveable* _ptr) {
	for (uint32 i = 0; i < saveData.size(); i++) {
		if (saveData[i].second == _ptr) { // If the pointer matches the argument data, deregister it
			saveData.erase(saveData.begin() + i);
			break; // Exit out of the loop
		}
	}
}