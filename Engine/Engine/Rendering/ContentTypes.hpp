#ifndef _RENDERING_CONTENT_TYPES_HPP
#define _RENDERING_CONTENT_TYPES_HPP
#include "../General/Types.hpp"

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

struct Character {
	float ax, ay;	// The character's advance values
	float bw, bh;	// The character's bitmap dimensions
	float bl, bt;	// The top-left coordinates for the character
	float tx;		// The x offset of the glyph in texture coordinates
};

class Font {

	struct Data {
		unsigned int refcount;
		unsigned int atlasID;
		float atlasWidth;
		float atlasHeight;
		map<char, Character> characters;
		string name;
	} *data;

public:

	// constructors
	Font() : data(nullptr) { }
	Font(map<char, Character> _characters, string _name, unsigned int _textureID, float _atlasWidth, float _atlasHeight);

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
	Character* GetCharacter(char value) const {
		if (data)	return &data->characters[value];
		else		return nullptr;
	}
	string GetName() const {
		if (data)	return data->name;
		else		return "";
	}
	unsigned int GetAtlasID() const {
		if (data)	return data->atlasID;
		else		return -1;
	}
	float GetAtlasWidth() const {
		if (data)	return data->atlasWidth;
		else		return -1;
	}
	float GetAtlasHeight() const {
		if (data)	return data->atlasHeight;
		else		return -1;
	}

private:

	// helper function
	void FreeThis();

};

/*class Sound {

	struct Data {
		unsigned int refcount;
		Mix_Chunk* sound;
	} *data;

public:
	// Constructors
	Sound() : data(nullptr) { }
	Sound(Mix_Chunk* _sound);

	// copy and move constructors/operators
	Sound(const Sound& other);
	Sound& operator=(const Sound& other);
	Sound(Sound&& other);
	Sound& operator=(Sound&& other);

	~Sound() { FreeThis(); }

	// getters
	bool IsValid() const { return data != nullptr; }
	unsigned int GetRefCount() const {
		if (data)	return data->refcount;
		else		return -1;
	}
	Mix_Chunk* GetSound() {
		if (data)	return data->sound;
		else		return nullptr;
	}

private:

	// helper function
	void FreeThis();
};*/

#endif // !_RENDERING_CONTENT_TYPES_HPP