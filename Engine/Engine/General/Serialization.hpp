#ifndef _SERIALIZATION_HPP
#define _SERIALIZATION_HPP

#include <nlohmann/json.hpp>
using nlohmann::json;

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
#include <rttr/variant_array_view.h>
#include <rttr/access_levels.h>
#include <rttr/policy.h>
using rttr::type;
using rttr::registration;
using rttr::method;
using rttr::property;
using rttr::constructor;
using rttr::destructor;
using rttr::variant;
using rttr::variant_array_view;
using rttr::access_levels;
using rttr::policy;

#define public_access_ rttr::detail::public_access()
#define protected_access_ rttr::detail::protected_access()
#define private_access_ rttr::detail::private_access()

#define public_object_constructor constructor<>()  (rttr::detail::as_raw_pointer(), public_access_)
#define protected_object_constructor constructor<>()  (rttr::detail::as_raw_pointer(), protected_access_)
#define private_object_constructor constructor<>()  (rttr::detail::as_raw_pointer(), private_access_)

#endif // !_SERIALIZATION_HPP