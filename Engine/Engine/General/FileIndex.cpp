#include "FileIndex.hpp"
#include "../Core/Debugger.hpp"
#include <fstream>

FileIndex::FileIndex(const string& file) {

	std::fstream fs(file);
	if (!fs.is_open()) {
		throw std::out_of_range("File: " + file + " was not found!");
	}

	// find the folder in the given file
	int32 i = 0;
	for (i = file.size() - 1; i >= 0; --i) {
		if (file[i] == '/' || file[i] == '\\')
			break;
	}
	// save the folder
	if (i > 0) folder = string(file.c_str(), i + 1); // make sure we copy the '/' too

	// read the file into a json object
	json j;
	try {
		fs >> j;
	} catch (json::parse_error& e) {
		DEBUG_ERROR(e.what());
	}

	fs.close();

	// read the json object into a map
	// its read exclusively as a (string, string) pair
	// any pairs that dont fit the format are skipped
	for (auto it = j.begin(); it != j.end(); ++it) {
		// move the values into our map
		index[it.key()] = std::move(it.value());
	}
}

FileIndex::~FileIndex() { }

//void FileIndex::PrintAll() const {
//	for (auto it : index) {
//		DEBUG_LOG("\"" + it.first);
//		// + "\" : \"" + it.second + "\""
//	}
//}

json FileIndex::GetJSON(const string& key) {
	try {
		json& value = index.at(key);
		// assume its a filepath
		if (value.is_string()) {
			string filepath = value.get<string>();
			std::fstream file(folder + filepath);
			if (!file.is_open()) {
				return json();
			}

			// load data into a json object
			json j;
			file >> j;
			file.close();
			// return 
			return j;
		}
		// return it as whatever json data it is
		else return index.at(key);
	}
	// catch if it doesnt find the key
	catch (std::out_of_range& e) {
		return json();
	}
	return json();
}

bool FileIndex::Contains(const string& key) {
	if (index.find(key) != index.end())
		return true;
	return false;
}

string FileIndex::GetJSONString(const string& key) {
	try {
		const json& j = index.at(key);
		if (j.is_string()) return j.get<std::string>();
		return string();
	}
	// catch if there is no key
	catch (std::out_of_range& e) {
		return string();
	}
}

string FileIndex::GetJSONPath(const string& key) {
	try {
		const json& j = index.at(key);
		if (j.is_string()) return folder + j.get<std::string>();
		return string();
	}
	// catch if there is no key
	catch (std::out_of_range&) {
		return string();
	}
}

FileIndex::const_iterator FileIndex::begin() const {
	return index.begin();
}

FileIndex::const_iterator FileIndex::end() const {
	return index.end();
}
