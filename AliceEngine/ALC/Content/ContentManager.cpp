#include "ContentManager.hpp"

namespace ALC {

	ContentStorage::ContentStorage() { }
	ContentStorage::~ContentStorage() {
		ContentManager::__RemoveContext(this);
		ContentManager::Clear(*this);
	}

	ContentStorage ContentManager::s_genericStorage;
	ContentStorage* ContentManager::s_contextStorage = nullptr;

	ContentStorage& ContentManager::Current() {
		if (s_contextStorage)
			return *s_contextStorage;
		return s_genericStorage;
	}
	
	ContentStorage& ContentManager::Default() {
		return s_genericStorage;
	}

	Texture ContentManager::LoadTexture(const string& path) {
		if (s_contextStorage)
			return LoadTexture(*s_contextStorage, path);
		return LoadTexture(s_genericStorage, path);
	}

	Texture ContentManager::LoadTexture(ContentStorage& storage, const string& path) {
		// check if it already exists
		auto it = storage.m_textures.find(path);
		if (it != storage.m_textures.end())
			return it->second;

		// load the texture and add it to the map
		Texture texture = Texture::Load(path);
		if (texture) storage.m_textures.emplace(path, texture);

		// return the newly loaded texture
		return texture;
	}

	Shader ContentManager::LoadShader(const string& path) {
		if (s_contextStorage)
			return LoadShader(*s_contextStorage, path);
		return LoadShader(s_genericStorage, path);
	}

	Shader ContentManager::LoadShader(ContentStorage& storage, const string& path) {
		// check if it already exists
		auto it = storage.m_shaders.find(path);
		if (it != storage.m_shaders.end())
			return it->second;

		// load the shader and add it to the map
		Shader shader = Shader::Load(path);
		if (shader) storage.m_shaders.emplace(path, shader);

		// return the newly loaded shader
		return shader;
	}

	Shader ContentManager::LoadShaderSource(const string& source) {
		if (s_contextStorage)
			return LoadShaderSource(*s_contextStorage, source);
		return LoadShaderSource(s_genericStorage, source);
	}

	Shader ContentManager::LoadShaderSource(ContentStorage& storage, const string& source) {
		// check if it already exists
		auto it = storage.m_shaders.find(source);
		if (it != storage.m_shaders.end())
			return it->second;

		// load the shader and add it to the map
		Shader shader = Shader::LoadSource(source);
		if (shader) storage.m_shaders.emplace(source, shader);

		// return the newly loaded shader
		return shader;
	}

	void ContentManager::Clear() {
		if (s_contextStorage)
			Clear(*s_contextStorage);
		Clear(s_genericStorage);
	}

	void ContentManager::Clear(ContentStorage& storage) {

		// iterate through and delete all of the textures
		for (auto& [key, texture] : storage.m_textures) {
			Texture::Delete(texture);
		}
		storage.m_textures.clear();

		// iterate through and delete all of the shaders
		for (auto& [key, shader] : storage.m_shaders) {
			Shader::Delete(shader);
		}
		storage.m_shaders.clear();

		//// iterate through and delete all of the fonts
		//for (auto& [key, font] : storage.m_fonts) {
		//	Font::Delete(font);
		//}
		//storage.m_fonts.clear();

	}

	void ContentManager::SetContext(ContentStorage& storage) {
		s_contextStorage = &storage;
	}

	void ContentManager::__RemoveContext(ContentStorage* storage) {
		// remove the storage only if it is the context
		if (s_contextStorage == storage)
			s_contextStorage = nullptr;
	}

}