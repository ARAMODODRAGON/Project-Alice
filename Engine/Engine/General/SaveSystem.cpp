#include "SaveSystem.hpp"
#include <sys/stat.h>
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
	DEBUG_ERROR("Could not save data to " + _fileName + ".json!");
	return false; // Couldn't save for some reason; return false
}

bool SaveSystem::LoadData(const string& _fileName) {
	json loadedfile;
	const char* appdata = getenv("APPDATA");
	string filePath(to_string(appdata) + "/Project-Alice");
	struct stat info;
	if (stat(filePath.c_str(), &info) != 0) { // The directory couldn't be accessed for whatever reason
		DEBUG_ERROR("Cannot access directory: " + filePath + "\nNo data will be loaded from file.");
		return false;
	}
	else if (info.st_mode & S_IFDIR) { // The directory exists and will be accessed for the save file
		ifstream stream(filePath + "/" + _fileName + ".json");
		if (stream.is_open()) { // Make sure the file exists before loading data from it
			stream >> loadedfile;
			for (uint32 i = 0; i < saveData.size(); i++) {
				saveData[i].second->LoadData(loadedfile);
			}
			stream.close();
			return true; // Load successful; return true
		}
	}
	DEBUG_ERROR("Given directory was not valid; no data will be loaded from the file: " + _fileName + ".json");
	return false; // Couldn't load for some reason; return false
}

void SaveSystem::Register(string _name, ISaveable* _ptr) {
	for (uint32 i = 0; i < saveData.size(); i++) {
		if (saveData[i].first == _name || saveData[i].second == _ptr) { // If the pair being added matches an existing pair OR there's a duplicate pointer, don't add it to the data
			DEBUG_WARNING("Data with an identical name OR the same pointer value has already been registered!");
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