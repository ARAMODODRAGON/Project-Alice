#ifndef ALC_CONTENT_SHADER_HPP
#define ALC_CONTENT_SHADER_HPP
#include "../General.hpp"

namespace ALC {

	struct Shader final {

		// default constructor
		Shader();

		// creates invalid shader
		// same as default constructor
		Shader(std::nullptr_t);

		// checks if this is a valid shader
		bool IsValid() const;

		// returns the shader ID
		operator uint32() const;

		// returns the shader ID
		uint32 GetID() const;

		// compares this to other
		bool operator==(const Shader& other) const;

		// compares this to other
		bool operator!=(const Shader& other) const;

		// returns the uniform location of the given name
		uint32 GetUniform(const string& name);

		// functions for loading shaders

		// loads a shader from source
		static Shader LoadSource(const string& source);

		// loads a shader from the given path
		static Shader Load(const string& path);

		// deletes the shader
		static void Delete(const Shader& shader);

	private:
		uint32 m_programID;
	};

}

#endif // !ALC_CONTENT_SHADER_HPP