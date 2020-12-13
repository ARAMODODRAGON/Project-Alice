#ifndef ALC_CONTENT_CONTENTMANAGER_HPP
#define ALC_CONTENT_CONTENTMANAGER_HPP
#include "../General.hpp"
#include "Texture.hpp"
#include "Shader.hpp"

namespace ALC {

	// used as a context to store textures/shaders/fonts
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

		// loads a texture and stores it in an internal map
		static Texture LoadTexture(const string& path);

		// loads a texture and stores it in the context
		static Texture LoadTexture(ContentStorage& context, const string& path);

		// loads a shader file and stores it in an internal map
		static Shader LoadShader(const string& path);
		
		// loads a shader file and stores it in the context
		static Shader LoadShader(ContentStorage& context, const string& path);

		// loads a shader source and stores it in an internal map
		static Shader LoadShaderSource(const string& source);

		// loads a shader source and stores it in the context
		static Shader LoadShaderSource(ContentStorage& context, const string& source);

		// loads a font file and stores it in a map
		// static Font LoadFont(const string& path);
		
		// loads a font file and stores it in the context
		// static Font LoadFont(ContentStorage& context, const string& path);

		// clears out and deletes the content stored in the content manager
		static void Clear();

		// clears out and deletes the content stored in the context
		static void Clear(ContentStorage& context);

	private:
		static ContentStorage s_genericStorage;
	};

}

#endif // !ALC_CONTENT_CONTENTMANAGER_HPP