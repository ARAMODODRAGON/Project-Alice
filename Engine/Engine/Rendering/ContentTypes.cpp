#include "ContentTypes.hpp"

////////////////////////// Texture /////////////////////////

Texture::Texture() : data(nullptr) { }

Texture::Texture(unsigned int id_, const uvec2& size_) : data(nullptr) {
	data = new Data();
	data->id = id_;
	data->size = size_;
	data->refcount = 1;
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

Texture::~Texture() { FreeThis(); }

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

Shader::Shader() : data(nullptr) { }

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

Shader::~Shader() { FreeThis(); }

void Shader::FreeThis() {
	if (data) {
		data->refcount--;
		if (data->refcount == 0) {
			delete data;
			data = nullptr;
		}
	}
}
