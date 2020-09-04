#include "ObjectIndex.hpp"
#include "../Core/Debugger.hpp"
#include <fstream>

#define INDEXFILE "/Index.json"

ObjectIndex::ObjectIndex(const string& folder_) : folder(folder_) {

	std::fstream file(folder_ + INDEXFILE);
	if (!file.is_open()) {
		DEBUG_ERROR("could not find Index.json in folder: " + folder_);
		return;
	}

	// read the file into a json object
	json j;
	try {
		file >> j;
	} catch (json::parse_error& e) {
		DEBUG_ERROR(e.what());
	}

	file.close();

	// read the json object into a map
	// its read exclusively as a (string, string) pair
	// any pairs that dont fit the format are skipped
	for (auto it = j.begin(); it != j.end(); ++it) {
		// map a value if it is a string
		if (it.value().is_string()) {
			index[it.key()] = it.value();
		} else {
			DEBUG_ERROR("Incorrectly mapped value: \"" + it.key() + "\"");
		}
	}
}

ObjectIndex::~ObjectIndex() { }

void ObjectIndex::PrintAll() const {
	for (auto it : index) {
		DEBUG_LOG("\"" + it.first + "\" : \"" + it.second + "\"");
	}
}

bool ObjectIndex::GetJSON(json* j, const string& name) {
	// try getting the filepath
	string filepath;
	try {
		filepath = folder + "/" + index.at(name);
	} catch (std::out_of_range&) {
		DEBUG_ERROR("Could not find object file with the name" + name);
		return false;
	}

	// try opening the file
	std::fstream file(filepath);
	if (!file.is_open()) {
		DEBUG_ERROR("could not find file with path: " + filepath);
		return false;
	}

	// load data into a json object
	file >> *j;
	file.close();

	return true;
}
