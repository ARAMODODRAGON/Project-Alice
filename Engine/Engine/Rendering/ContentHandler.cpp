#include "ContentHandler.hpp"
#include <SDL_image.h>
#include <glew.h>
#include <algorithm>
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
	for (auto& fp : fonts) {
		GLuint id = fp.second.GetAtlasID();
		glDeleteTextures(1, &id);
	}
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

Font ContentHandler::LoadFont(const string& fontName, int fontSize) {
	auto& fonts = Get()->fonts;
	auto* fontIndex = Get()->fontIndex;

	// Check if the font has already been loaded; return if it has been
	if (fonts.find(fontName) != fonts.end()) {
		return fonts[fontName];
	}

	string path = Get()->fontIndex->GetPath(fontName);
	FT_Face face = ::LoadFont(path, 0, fontSize);
	FT_GlyphSlot g = face->glyph;

	int w = 0, h = 0;
	for (int i = 32; i < 128; i++) {
		if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
			DEBUG_ERROR("Failed to load glyph at index: " + VTOS(i));
			continue;
		}
		w += g->bitmap.width;
		// I (dom) changed this because max is no longer a macro, I removed the header it was from
		//h = max(h, g->bitmap.rows);
		if (h < g->bitmap.rows)
			h = g->bitmap.rows;
	}

	unsigned int texture = -1;
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	map<char, Character> characters;
	float x = 0;
	for (int i = 32; i < 128; i++) {
		if (FT_Load_Char(face, i, FT_LOAD_RENDER)) { // Skip over the elements that aren't properly loaded
			continue;
		}
		glTexSubImage2D(GL_TEXTURE_2D, 0, x, 0, g->bitmap.width, g->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);

		Character character{};
		character.ax = g->advance.x >> 6;
		character.ay = g->advance.y >> 6;
		character.bw = g->bitmap.width;
		character.bh = g->bitmap.rows;
		character.bl = g->bitmap_left;
		character.bt = g->bitmap_top;
		character.tx = x / w;

		characters.insert(pair<char, Character>(i, character));
		x += g->bitmap.width;
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	FT_Done_Face(face);

	// Create the font, store it, and return the data
	Font font(characters, texture, w, h);
	fonts.insert(FontPairType(fontName, font));
	return font;
}
