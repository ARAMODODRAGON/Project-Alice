#include "ContentLoadFunctions.hpp"
#include <fstream>
#include "../Core/Debugger.hpp"
#include <glew.h>
#include <SDL_image.h>

unsigned int LoadTexture(const string& path, uvec2& outSize) {

	// load surface
	SDL_Surface* surface = IMG_Load(path.c_str());
	if (surface == nullptr) {
		DEBUG_ERROR("Could not find file: " + path);
		return -1;
	}

	// grab the size
	outSize = uvec2(surface->w, surface->h);

	// create and bind a texture object
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// check the pixel format of the image
	//DEBUG_LOG("bytes per pixel: " + VTOS(surface->format->BytesPerPixel));
	int mode = GL_RGBA;
	// contains an alpha channel
	if (surface->format->BytesPerPixel == 4) {
		if (surface->format->Rmask == 0x000000ff)
			mode = GL_RGBA;
		else
			mode = GL_BGRA;
	}
	// no alpha channel
	else if (surface->format->BytesPerPixel == 3) {
		if (surface->format->Rmask == 0x000000ff)
			mode = GL_RGB;
		else
			mode = GL_BGR;
	} else {
		DEBUG_ERROR("No matching pixel format!");
		SDL_FreeSurface(surface);
		glDeleteTextures(1, &textureID);
		return -1;
	}

	// load in the texture
	glTexImage2D(GL_TEXTURE_2D, 0, mode, surface->w, surface->h, 0, mode, GL_UNSIGNED_BYTE, surface->pixels);

	// Wrapping and filtering options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// delete the SDL_Surface and unbind the texture
	SDL_FreeSurface(surface);
	glBindTexture(GL_TEXTURE_2D, 0);

	// return
	return textureID;
}

string LoadSource(const string& path) {
	// file stream
	std::ifstream stream;
	stream.open(path);

	if (!stream.is_open()) {
		// failed to open file
		DEBUG_ERROR("Failed to open file " + path);
		stream.close();
		return "";
	}

	// stores the code
	string source, line;

	// read every line from the file
	// add \0 to the end to tell the compiler that this is the end of the file
	while (getline(stream, line))
		source.append(line).append("\n");
	source.append("\0");

	// finish and return
	stream.close();
	return source;
}

unsigned int GetShaderType(const string& path) {
	// grab just the extention
	auto pointPos = path.begin();
	for (auto it = path.begin(); it != path.end(); ++it) {
		if (*it == '.') pointPos = it;
	}
	string extention(pointPos, path.end());

	// vertex shader
	if (extention == ".vert")
		return GL_VERTEX_SHADER;
	// fragment shader
	else if (extention == ".frag")
		return GL_FRAGMENT_SHADER;
	// geometry shader
	else if (extention == ".geom")
		return GL_GEOMETRY_SHADER;
	// compute shader
	else if (extention == ".comp")
		return GL_COMPUTE_SHADER;

	return -1;
}

unsigned int LoadShader(const string& path) {

	// check if the shader type is supported
	GLuint shaderType = GetShaderType(path);
	if (shaderType == -1) {
		DEBUG_ERROR("Shader could not be loaded as its not a valid shader type: " + path);
		return -1;
	}

	// load the shader
	string source = LoadSource(path);
	const char* csource = source.c_str();
	GLuint shaderID = glCreateShader(shaderType);
	// load and compile the code
	glShaderSource(shaderID, 1, &csource, 0);
	glCompileShader(shaderID);

	// check for errors
	GLint success;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
	if (!success) {
		// get error and print
		GLchar infoLog[512];
		glGetShaderInfoLog(shaderID, 512, 0, infoLog);
		DEBUG_ERROR("Failed to compile shader: " + string(infoLog));

		// delete the shader and return
		glDeleteShader(shaderID);
		return -1;
	}

	return shaderID;
}

unsigned int LoadShaderProgram(const vector<string>& shaderPaths) {
	if (shaderPaths.size() == 0) {
		DEBUG_ERROR("No shaders to load");
		return -1;
	}

	// create shader
	GLuint shaderProgram = glCreateProgram();

	vector<GLuint> shaders;
	shaders.reserve(4);
	for (const string& path : shaderPaths) {
		// load the shader
		GLuint shaderID = LoadShader(path);
		// skip if its not valid
		if (shaderID == -1) continue;

		// attach to program
		glAttachShader(shaderProgram, shaderID);
		shaders.push_back(shaderID);
	}

	// no shaders were loaded so delete the shaders/program
	if (shaders.size() == 0) {
		DEBUG_ERROR("Could not load any shaders, deleted shader program");
		for (GLuint id : shaders) glDeleteShader(id);
		glDeleteProgram(shaderProgram);
		return -1;
	}

	// link
	glLinkProgram(shaderProgram);

	// check linking errors
	GLint success;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		// get error message and print
		GLchar infoLog[512];
		glGetProgramInfoLog(shaderProgram, 512, 0, infoLog);
		DEBUG_ERROR("Failed to link program: " + string(infoLog));
		// delete shaders/program
		for (GLuint id : shaders) glDeleteShader(id);
		glDeleteProgram(shaderProgram);
		return -1;
	}

	// delete shaders
	for (GLuint id : shaders) glDeleteShader(id);

	// return 
	return shaderProgram;
}

FT_Face LoadFont(const string& path, const int width, const int height) {
	const char* fontPath = path.c_str();
	FT_Face face; // Attempt to store the face data for the specified font
	if (FT_New_Face(Game::Get()->GetFontLibrary(), fontPath, 0, &face)) {
		DEBUG_ERROR("Failed to load the font at path " + path);
		return NULL;
	}
	FT_Set_Pixel_Sizes(face, width, height);
	return face;
}
