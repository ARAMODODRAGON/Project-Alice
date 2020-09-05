#ifndef _CORE_OBJECT_INDEX_HPP
#define _CORE_OBJECT_INDEX_HPP
#include "../General/Types.hpp"
#include "../General/Serialization.hpp"

class ObjectIndex {

	// contains all object names with the path to the json file
	map<string, string> index;
	string folder;

public:

	// the folder must contain a file titled "Index.json" that contains all of the objects
	ObjectIndex(const string& folder_);
	~ObjectIndex();

	// functions
	void PrintAll() const;
	bool GetJSON(json* j, const string& name);
	bool Contains(const string& name);

};

#endif // !_CORE_OBJECT_INDEX_HPP