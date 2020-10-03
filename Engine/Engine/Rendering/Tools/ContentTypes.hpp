#ifndef _RENDERING_CONTENT_TYPES_HPP
#define _RENDERING_CONTENT_TYPES_HPP
#include "../../General/Types.hpp"

class Texture {

	struct Data {
		unsigned int id;
		vec2 size;
		unsigned int refcount;
		string name;
	} *data;

public:

	// constructors
	Texture() : data(nullptr) { }
	Texture(unsigned int id_, const vec2& size_, const string& name_);

	// copy and move constructors/operators
	Texture(const Texture& other);
	Texture& operator=(const Texture& other);
	Texture(Texture&& other);
	Texture& operator=(Texture&& other);

	~Texture() { FreeThis(); }

	// functions
	bool IsValid() const { return data != nullptr; }
	unsigned int GetRefCount() const {
		if (data)	return data->refcount;
		else		return -1;
	}
	string GetName() const {
		if (data)	return data->name;
		else		return "";
	}
	unsigned int GetID() const {
		if (data)	return data->id;
		else		return -1;
	}
	operator unsigned int() const {
		if (data)	return data->id;
		else		return -1;
	}
	vec2 GetSize() const {
		if (data)	return data->size;
		else		return vec2();
	}

private:

	// helper
	void FreeThis();

};

class Shader {

	struct Data {
		unsigned int id;
		unsigned int refcount;
	} *data;

public:

	// constructors
	Shader() : data(nullptr) { }
	Shader(unsigned int id_);

	// copy and move constructors/operators
	Shader(const Shader& other);
	Shader& operator=(const Shader& other);
	Shader(Shader&& other);
	Shader& operator=(Shader&& other);

	~Shader() { FreeThis(); }

	// functions
	bool IsValid() const { return data != nullptr; }
	unsigned int GetRefCount() const {
		if (data)	return data->refcount;
		else		return -1;
	}
	unsigned int GetID() const {
		if (data)	return data->id;
		else		return -1;
	}
	operator unsigned int() const {
		if (data)	return data->id;
		else		return -1;
	}

private:

	// helper
	void FreeThis();

};

class Font {

	struct Data {
		unsigned int refcount;
		// member variables go in here
	} *data;

public:

	// constructors
	Font() : data(nullptr) { }
	// Font(arguments go here);

	// copy and move constructors/operators
	Font(const Font& other);
	Font& operator=(const Font& other);
	Font(Font&& other);
	Font& operator=(Font&& other);

	// getters and setters
	~Font() { FreeThis(); }

	// getters
	bool IsValid() const { return data != nullptr; }
	unsigned int GetRefCount() const {
		if (data)	return data->refcount;
		else		return -1;
	}

private:

	// helper function
	void FreeThis();

};

#endif // !_RENDERING_CONTENT_TYPES_HPP