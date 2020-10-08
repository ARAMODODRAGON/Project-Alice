#include "ContentTypes.hpp"

////////////////////////// Texture /////////////////////////

Texture::Texture(unsigned int id_, const vec2& size_, const string& name_) : data(nullptr) {
	data = new Data();
	data->id = id_;
	data->size = size_;
	data->refcount = 1;
	data->name = name_;
}

Texture::Texture(const Texture& other) : data(other.data) {
	data->refcount++;
}
Texture& Texture::operator=(const Texture& other) {
	FreeThis();
	data = other.data;
	if (data) data->refcount++;
	return *this;
}
Texture::Texture(Texture&& other) : data(other.data) {
	other.data = nullptr;
}
Texture& Texture::operator=(Texture&& other) {
	FreeThis();
	data = other.data;
	other.data = nullptr;
	return *this;
}

void Texture::FreeThis() {
	if (data) {
		data->refcount--;
		if (data->refcount == 0) {
			delete data;
			data = nullptr;
		}
	}
}

////////////////////////// Shader //////////////////////////

Shader::Shader(unsigned int id_) : data(nullptr) {
	data = new Data();
	data->id = id_;
	data->refcount = 1;
}

Shader::Shader(const Shader& other) : data(other.data) {
	data->refcount++;
}
Shader& Shader::operator=(const Shader& other) {
	FreeThis();
	data = other.data;
	if (data) data->refcount++;
	return *this;
}
Shader::Shader(Shader&& other) : data(other.data) {
	other.data = nullptr;
}
Shader& Shader::operator=(Shader&& other) {
	FreeThis();
	data = other.data;
	other.data = nullptr;
	return *this;
}

void Shader::FreeThis() {
	if (data) {
		data->refcount--;
		if (data->refcount == 0) {
			delete data;
			data = nullptr;
		}
	}
}

////////////////////////// Font //////////////////////////

Font::Font(map<char, Character> _characters, string _name, unsigned int _textureID, float _atlasWidth, float _atlasHeight) {
	data = new Data();
	data->characters = _characters;
	data->atlasID = _textureID;
	data->name = _name;
	data->atlasWidth = _atlasWidth;
	data->atlasHeight = _atlasHeight;
	data->refcount = 1;
}

Font::Font(const Font& other) : data(other.data) {
	data->refcount++;
}
Font& Font::operator=(const Font& other) {
	FreeThis();
	data = other.data;
	data->refcount++;
	return *this;
}
Font::Font(Font&& other) : data(other.data) { 
	other.data = nullptr;
}
Font& Font::operator=(Font&& other) {
	FreeThis();
	data = other.data;
	other.data = nullptr;
	return *this;
}

void Font::FreeThis() {
	if (data) {
		data->refcount--;
		if (data->refcount == 0) {
			delete data;
			data = nullptr;
		}
	}
}
