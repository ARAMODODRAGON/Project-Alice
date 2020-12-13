#ifndef ALC_CONTENT_CONTENTMANAGER_HPP
#define ALC_CONTENT_CONTENTMANAGER_HPP
#include "../General.hpp"
#include "Texture.hpp"
#include "Shader.hpp"

namespace ALC {

	// used as storage to store textures/shaders/fonts
	// cannot be directly interacted with
	class ContentStorage final {
		ALC_NO_COPY(ContentStorage);
	public:

		ContentStorage();
		~ContentStorage();

	private:
		friend class ContentManager;
		unordered_map<string, Texture> m_textures;
		unordered_map<string, Shader> m_shaders;
	};

	class ContentManager final {
		ALC_NON_CONSTRUCTABLE(ContentManager)
	public:

		// loads a texture and stores it in an internal storage, or the set context
		static Texture LoadTexture(const string& path);

		// loads a texture and stores it in the storage
		static Texture LoadTexture(ContentStorage& storage, const string& path);

		// loads a shader file and stores it in an internal storage, or the set context
		static Shader LoadShader(const string& path);
		
		// loads a shader file and stores it in the storage
		static Shader LoadShader(ContentStorage& storage, const string& path);

		// loads a shader source and stores it in an internal storage, or the set context
		static Shader LoadShaderSource(const string& source);

		// loads a shader source and stores it in the storage
		static Shader LoadShaderSource(ContentStorage& storage, const string& source);

		// loads a font file and stores it in an internal storage, or the set context
		// static Font LoadFont(const string& path);
		
		// loads a font file and stores it in the storage
		// static Font LoadFont(ContentStorage& storage, const string& path);

		// clears out and deletes the content stored in the content manager
		static void Clear();

		// clears out and deletes the content stored in the storage
		static void Clear(ContentStorage& storage);

		// sets the passed in storage as the default storage to load into
		// any content loaded before setting the storage does not move to this storage
		static void SetContext(ContentStorage& context);

	private:
		static ContentStorage s_genericStorage;
		static ContentStorage* s_contextStorage;
	public:
		static void __RemoveContext(ContentStorage* context);
	};

}

#endif // !ALC_CONTENT_CONTENTMANAGER_HPP