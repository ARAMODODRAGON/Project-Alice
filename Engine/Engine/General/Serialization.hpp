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

#endif // !_SERIALIZATION_HPP