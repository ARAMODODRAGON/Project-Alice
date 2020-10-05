#include "ContentHandler.hpp"
#include <SDL_image.h>
#include <glew.h>
#include "ContentLoadFunctions.hpp"
#include "../Core/Debugger.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H

ContentHandler::ContentHandler()
	: textureIndex(nullptr), shaderIndex(nullptr) { }

ContentHandler::~ContentHandler() { 
	if (textureIndex) delete textureIndex;
	textureIndex = nullptr; 
	if (shaderIndex) delete shaderIndex;
	shaderIndex = nullptr;
}

void ContentHandler::Init(const string& textureIndexPath_, const string& shaderIndexPath_, const string& fontIndexPath_) {
	Get()->textureIndex = new FileIndex(textureIndexPath_);
	Get()->shaderIndex = new FileIndex(shaderIndexPath_);
	Get()->fontIndex = new FileIndex(fontIndexPath_);
}

void ContentHandler::Clean() {
	if (Get()->textureIndex == Get()->shaderIndex) {
		DEBUG_ERROR("ContentHandler has not been initialized");
		return;
	}

	auto& textures = Get()->textures;
	auto& shaders = Get()->shaders;
	auto& fonts = Get()->fonts;

	// delete textures
	for (auto it = textures.begin(); it != textures.end(); ++it) {
		if (it->second.GetRefCount() == 1) {
			GLuint id = it->second;
			glDeleteTextures(1, &id);
			it = textures.erase(it);
			// special case
			if (it == textures.end()) break;
		}
	}

	// delete shaders
	for (auto it = shaders.begin(); it != shaders.end(); ++it) {
		if (it->second.GetRefCount() == 1) {
			glDeleteProgram(it->second);
			it = shaders.erase(it);
		}
	}

	// delete fonts
	for (auto it = fonts.begin(); it != fonts.end(); ++it) {
		if (it->second.GetRefCount() == 1) {
			//DEBUG_ERROR("Cant clean up fonts!");
			it = fonts.erase(it);
		}
	}
}

void ContentHandler::Exit() {
	if (Get()->textureIndex == Get()->shaderIndex) {
		DEBUG_ERROR("ContentHandler has not been initialized");
		return;
	}

	auto& textures = Get()->textures;
	auto& shaders = Get()->shaders;
	auto& fonts = Get()->fonts;

	// delete textures
	for (auto& tp : textures) {
		GLuint id = tp.second;
		glDeleteTextures(1, &id);
	}
	textures.clear();

	// delete shaders
	for (auto& sp : shaders)
		glDeleteProgram(sp.second);
	shaders.clear();

	// delete fonts
	/*for (auto& fp : fonts) {
		DEBUG_ERROR("Cant delete font!");
	}*/
	fonts.clear();
}

Texture ContentHandler::LoadTexture(const string& textureName) {
	if (Get()->textureIndex == Get()->shaderIndex) {
		DEBUG_ERROR("ContentHandler has not been initialized");
		return Texture();
	}
	auto& textures = Get()->textures;
	auto* textureIndex = Get()->textureIndex;

	// first check if the texture is already loaded
	if (textures.find(textureName) != textures.end())
		return textures[textureName];

	// get the path
	string path = textureIndex->GetPath(textureName);

	// load the texture
	uvec2 size;
	GLuint textureID = ::LoadTexture(path, size);

	// check if it failed
	if (textureID == -1) return Texture();

	// create texture and return
	Texture texture(textureID, size, textureName);
	textures.insert(TexturePairType(textureName, texture));
	return texture;
}

Shader ContentHandler::LoadShader(const string& shaderName) {
	if (Get()->textureIndex == Get()->shaderIndex) {
		DEBUG_ERROR("ContentHandler has not been initialized");
		return Shader();
	}
	auto& shaders = Get()->shaders;
	auto* shaderIndex = Get()->shaderIndex;

	// check if the shader is already loaded
	if (shaders.find(shaderName) != shaders.end())
		return shaders[shaderName];

	GLuint shaderID = -1;

	// get all shader paths
	json j;
	shaderIndex->GetJSON(&j, shaderName);
	if (j.contains("shaders")) {
		json& arr = j["shaders"];
		if (arr.is_array()) {

			vector<string> shaderPaths;
			shaderPaths.reserve(4);
			for (size_t i = 0; i < arr.size(); ++i) {
				if (arr[i].is_string()) {
					shaderPaths.push_back(arr[i].get<string>());
				}
			}

			shaderID = ::LoadShaderProgram(shaderPaths);
		}
	}

	// check if it failed
	if (shaderID == -1) return Shader();

	// create shader and return
	Shader shader(shaderID);
	shaders.insert(ShaderPairType(shaderName, shader));
	return shader;
}

Font ContentHandler::LoadFont(const string& fontName) {
	auto& fonts = Get()->fonts;
	auto* fontIndex = Get()->fontIndex;

	// Check if the font has already been loaded; return if it has been
	if (fonts.find(fontName) != fonts.end()) {
		return fonts[fontName];
	}

	string path = Get()->fontIndex->GetPath(fontName);
	FT_Face face = ::LoadFont(path, 0, 12);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	map<char, Character> characters;
	for (unsigned int i = 0; i < 128; i++) {
		if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
			string message = "Failed to load current glyph.";
			DEBUG_ERROR(message);
			continue; // Move onto the next available glyph
		}
		// Generate a texture for the glyph
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
		// Set texture's options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Now store the character in the font map
		Character character = {
			texture,
			face->glyph->advance.x,
			ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top)
		};
		characters.insert(pair<char, Character>(i, character));
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 0);
	FT_Done_Face(face);

	// Create the font, store it, and return the data
	Font font(characters);
	fonts.insert(FontPairType(fontName, font));
	return font;
}
