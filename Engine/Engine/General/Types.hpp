#ifndef _GENERAL_TYPES_HPP
#define _GENERAL_TYPES_HPP

// we want a bunch of integer types
#include <inttypes.h>
typedef uint64_t uint64;
typedef uint32_t uint32;
typedef uint16_t uint16;
typedef int64_t  int64;
typedef uint8_t  uint8;
typedef int32_t  int32;
typedef int16_t  int16;
typedef int8_t   int8;

typedef uint8_t ubyte;

#include <string>
#include <array>
#include <vector>
#include <list>
#include <forward_list>
#include <map>
#include <bitset>
#include <utility>
using std::string;
using std::to_string;
using std::array;
using std::vector;
using std::list;
using std::forward_list;
using std::map;
using std::bitset;
using std::pair;

#include "Math.hpp"

typedef unsigned char ubyte;

#endif // !_GENERAL_TYPES_HPP