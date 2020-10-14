#ifndef _CORE_INDEX_HPP
#define _CORE_INDEX_HPP
#include "../General/Types.hpp"
#include "../General/Serialization.hpp"

class FileIndex {
public:

	// the map used within the fileindex
	using maptype = map<string, json>;
	// the only iterator available for iterating through the internal map
	using const_iterator = maptype::const_iterator;

private:

	// contains all object names with the path to the json file
	maptype index;
	string folder;

public:

	// the folder must contain a file titled "Index.json" that contains all of the objects
	FileIndex(const string& file_);
	~FileIndex();

	// functions
	
	// prints all of the items in this FileIndex
	//void PrintAll() const;

	// returns the json data at that key
	// if the json data is a string then its assumed that the value points to another json file
	json GetJSON(const string& key);

	// returns if that key exists
	bool Contains(const string& key);

	// returns the string at that index
	string GetJSONString(const string& key);
	
	// returns the string at that index as a filepath starting at the index file
	string GetJSONPath(const string& key);

	// iteration through the fileindex
	const_iterator begin() const;
	const_iterator end() const;

};

#endif // !_CORE_INDEX_HPP