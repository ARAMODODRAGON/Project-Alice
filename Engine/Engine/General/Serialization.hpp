#ifndef _SERIALIZATION_HPP
#define _SERIALIZATION_HPP

#include <nlohmann/json.hpp>
using nlohmann::json;

// need to remove these 
#undef max
#undef min

#include "Types.hpp"

inline uint32 __TypeCounter() {
	static uint32 counter = 0;
	return counter++;
}

template<class T>
inline uint32 GetTypeID() {
	static const uint32 ID = __TypeCounter();
	return ID;
}

/*

template<typename... ARGS>
constexpr size_t count_args(ARGS&&... args) {
return sizeof...(ARGS);
}
#define ALFIRST(first, second) first
#define ALENUM(valuetype, name, ...)												\
class name {																		\
valuetype value;																\
public:																				\
name() { }																		\
name(const valuetype value_) : value(value_) { }								\
operator valuetype() const { return value; }									\
name& operator=(const valuetype value_) { value = value_; return *this; }		\
static constexpr size_t count() {												\
return count_args(__VA_ARGS__);												\
}																				\
static constexpr const char* type_name() { return #name; }						\
enum : valuetype { __VA_ARGS__ };												\
}

#define ALENUM_U32(name, ...) ALENUM(uint32, name, __VA_ARGS__)
#define ALENUM_U64(name, ...) ALENUM(uint64, name, __VA_ARGS__)

ALENUM_U32(Test, test1, test2);

*/

#endif // !_SERIALIZATION_HPP