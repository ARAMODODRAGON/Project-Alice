#include "Math.hpp"
#include "Serialization.hpp"

//glm::vec2
//glm::vec3
//glm::vec4

//glm::ivec2
//glm::ivec3
//glm::ivec4

//glm::uvec2
//glm::uvec3
//glm::uvec4

//glm::mat3
//glm::mat4

// register all the glm types ill be using
RTTR_REGISTRATION {
	registration::class_<glm::vec2>("glm::vec2")
		.constructor<>()
		.property("x", &glm::vec2::x)
		.property("y", &glm::vec2::y);
	registration::class_<glm::vec3>("glm::vec3")
		.constructor<>()
		.property("x", &glm::vec3::x)
		.property("y", &glm::vec3::y)
		.property("z", &glm::vec3::z);
	registration::class_<glm::vec4>("glm::vec4")
		.constructor<>()
		.property("x", &glm::vec4::x)
		.property("y", &glm::vec4::y)
		.property("z", &glm::vec4::z)
		.property("w", &glm::vec4::w);

	registration::class_<glm::ivec2>("glm::ivec2")
		.constructor<>()
		.property("x", &glm::ivec2::x)
		.property("y", &glm::ivec2::y);
	registration::class_<glm::ivec3>("glm::ivec3")
		.constructor<>()
		.property("x", &glm::ivec3::x)
		.property("y", &glm::ivec3::y)
		.property("z", &glm::ivec3::z);
	registration::class_<glm::ivec4>("glm::ivec4")
		.constructor<>()
		.property("x", &glm::ivec4::x)
		.property("y", &glm::ivec4::y)
		.property("z", &glm::ivec4::z)
		.property("w", &glm::ivec4::w);

	registration::class_<glm::uvec2>("glm::uvec2")
		.constructor<>()
		.property("x", &glm::uvec2::x)
		.property("y", &glm::uvec2::y);
	registration::class_<glm::uvec3>("glm::uvec3")
		.constructor<>()
		.property("x", &glm::uvec3::x)
		.property("y", &glm::uvec3::y)
		.property("z", &glm::uvec3::z);
	registration::class_<glm::uvec4>("glm::uvec4")
		.constructor<>()
		.property("x", &glm::uvec4::x)
		.property("y", &glm::uvec4::y)
		.property("z", &glm::uvec4::z)
		.property("w", &glm::uvec4::w);

	registration::class_<glm::mat3>("glm::mat3")
		.constructor<>();
	registration::class_<glm::mat4>("glm::mat4")
		.constructor<>();
}
