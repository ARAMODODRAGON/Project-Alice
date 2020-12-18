#ifndef ALC_GENERAL_HPP
#define ALC_GENERAL_HPP
#include <string>
//#include <string_view>
#include <array>
#include <vector>
#include <list>
#include <forward_list>
#include <map>
#include <unordered_map>
#include <utility>
#include <memory>
#include <functional>
#define GLM_FORCE_RADIANS
#include <glm\glm.hpp>
#undef assert
#include <assert.h>
#ifdef NDEBUG
#define ALC_ASSERT(condition, text)
#else
#define ALC_ASSERT(condition, text) assert(condition)
#endif

#define ALC_PI 3.141592653f
#define ALC_TO_RADIANS(degrees) (degrees * (ALC_PI / 180.0f))
#define ALC_TO_DEGREES(radians) (radians * (180.0f / ALC_PI))


namespace ALC {

	using std::string;
	//using std::string_view;
	using std::array;
	using std::vector;
	using std::list;
	using std::forward_list;
	using std::map;
	using std::unordered_map;
	using std::pair;
	using std::function;

	// using statement for std::shared_ptr<T>
	template<typename T>
	using Ref = std::shared_ptr<T>;

	// using statement for std::unique_ptr<T>
	template<typename T>
	using Scope = std::unique_ptr<T>;

	using uint64 = uint64_t;
	using uint32 = uint32_t;
	using uint16 = uint16_t;
	using uint8 = uint8_t;
	using int64 = int64_t;
	using int32 = int32_t;
	using int16 = int16_t;
	using int8 = int8_t;

	using glm::vec2;
	using glm::vec3;
	using glm::vec4;
	using glm::ivec2;
	using glm::ivec3;
	using glm::ivec4;
	using glm::uvec2;
	using glm::uvec3;
	using glm::uvec4;
	using glm::mat3;
	using glm::mat4;

	// colors

	#define ALC_COLOR_CLEAR ::ALC::vec4(0.0f, 0.0f, 0.0f, 0.0f)
	#define ALC_COLOR_BLACK ::ALC::vec4(0.0f, 0.0f, 0.0f, 1.0f)
	#define ALC_COLOR_WHITE ::ALC::vec4(1.0f, 1.0f, 1.0f, 1.0f)
	#define ALC_COLOR_RED	::ALC::vec4(1.0f, 0.0f, 0.0f, 1.0f)
	#define ALC_COLOR_GREEN ::ALC::vec4(0.0f, 1.0f, 0.0f, 1.0f)
	#define ALC_COLOR_BLUE	::ALC::vec4(0.0f, 0.0f, 1.0f, 1.0f)


	#define DEFAULT_PERCISION 0.0001f
	inline bool NearlyZero(const float& v, const float& percision = DEFAULT_PERCISION) {
		return (abs(v) < percision);
	}
	inline bool NearlyEqual(const float& v0, const float& v1, const float& percision = DEFAULT_PERCISION) {
		return (abs(v0 - v1) < percision);
	}

	inline bool NearlyZero(const vec2& v, const float& percision = DEFAULT_PERCISION) {
		return (abs(v.x) < percision)
			&& (abs(v.y) < percision);
	}
	inline bool NearlyEqual(const vec2& v0, const vec2& v1, const float& percision = DEFAULT_PERCISION) {
		return (abs(v0.x - v1.x) < percision)
			&& (abs(v0.y - v1.y) < percision);
	}

	inline bool NearlyZero(const vec3& v, const float& percision = DEFAULT_PERCISION) {
		return (abs(v.x) < percision)
			&& (abs(v.y) < percision)
			&& (abs(v.z) < percision);
	}
	inline bool NearlyEqual(const vec3& v0, const vec3& v1, const float& percision = DEFAULT_PERCISION) {
		return (abs(v0.x - v1.x) < percision)
			&& (abs(v0.y - v1.y) < percision)
			&& (abs(v0.z - v1.z) < percision);
	}

	inline bool NearlyZero(const vec4& v, const float& percision = DEFAULT_PERCISION) {
		return (abs(v.x) < percision)
			&& (abs(v.y) < percision)
			&& (abs(v.z) < percision)
			&& (abs(v.w) < percision);
	}
	inline bool NearlyEqual(const vec4& v0, const vec4& v1, const float& percision = DEFAULT_PERCISION) {
		return (abs(v0.x - v1.x) < percision)
			&& (abs(v0.y - v1.y) < percision)
			&& (abs(v0.z - v1.z) < percision)
			&& (abs(v0.w - v1.w) < percision);
	}
	#undef DEFAULT_PERCISION

}

#include "rect.hpp"

// disables the copy constructor and operator
#define ALC_NO_COPY(TYPE)				\
TYPE(const TYPE&) = delete;				\
TYPE& operator=(const TYPE&) = delete;		

// disables the move constructor and operator
#define ALC_NO_MOVE(TYPE)				\
TYPE(TYPE&&) = delete;					\
TYPE& operator=(TYPE&&) = delete;

// disables copy/move constructors and operators
// and disables the default constructor and destructor
#define ALC_NON_CONSTRUCTABLE(TYPE)		\
ALC_NO_COPY(TYPE) ALC_NO_MOVE(TYPE)		\
TYPE() = delete;						\
~TYPE() = delete;

#define ALC_SINGLETON(TYPE)								\
ALC_NO_COPY(TYPE) ALC_NO_MOVE(TYPE)						\
public:													\
static TYPE* Get() {									\
	static ::ALC::Scope<TYPE> _instance(new TYPE());	\
	return &_instance.get();							\
}														\
private:

// declares that the class/function/variable is internal to the 
// library and should not be accessed by any external source
#define ALC_INTERNAL

#include "Core\Debugger.hpp"

#endif // !ALC_GENERAL_HPP