#ifndef _CORE_OBJECT_INDEX_HPP
#define _CORE_OBJECT_INDEX_HPP
#include "../General/Types.hpp"
#include "../General/Serialization.hpp"

class FileIndex {

	// contains all object names with the path to the json file
	map<string, string> index;
	string folder;

public:

	using iterator = map<string, string>::iterator;

	// the folder must contain a file titled "Index.json" that contains all of the objects
	FileIndex(const string& folder_);
	~FileIndex();

	// functions
	void PrintAll() const;
	bool GetJSON(json* j, const string& name);
	bool Contains(const string& name);
	string GetPath(const string& name);
	string GetRelativePath(const string& name);

	// iteration
	iterator begin();
	iterator end();

};

#endif // !_CORE_OBJECT_INDEX_HPP