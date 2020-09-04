#ifndef _SERIALIZATION_HPP
#define _SERIALIZATION_HPP

#include <nlohmann/json.hpp>
using namespace nlohmann;

// need to remove these since RTTR uses these words differently
#undef max
#undef min

#include <rttr/type.h>
#include <rttr/rttr_enable.h>
#include <rttr/registration.h>
#include <rttr/method.h>
#include <rttr/property.h>
#include <rttr/constructor.h>
#include <rttr/destructor.h>
#include <rttr/variant.h>
#include <rttr/access_levels.h>
#include <rttr/policy.h>
using namespace rttr;

#endif // !_SERIALIZATION_HPP