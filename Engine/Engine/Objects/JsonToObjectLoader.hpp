#ifndef _OBJECTS_JSON_TO_OBJECT_LOADER_HPP
#define _OBJECTS_JSON_TO_OBJECT_LOADER_HPP
#include "../General/Serialization.hpp"
#include "../Core/Debugger.hpp"

void JsonToObject(rttr::type ty, variant& object, const json& j);
void SetPropertyFromValue(variant& object, property& prop, const json::value_t& jtype, const json& value);

inline void SetPropertyFromValue(variant& object, property& prop, const json::value_t& jtype, const json& value) {
	switch (jtype) {
		case json::value_t::null:
			prop.set_value(object, {});
			break;
		case json::value_t::object:
		{
			variant subobject = prop.get_value(object);

			// recurse
			JsonToObject(subobject.get_type(), subobject, value);

			if (!prop.set_value(object, subobject))
				DEBUG_ERROR("Failed to set value");
			break;
		}
		case json::value_t::array:
		{
			// TODO: initialize array
			DEBUG_WARNING("Cannot initialize arrays");

			break;
		}
		case json::value_t::string:
			if (!prop.set_value(object, value.get<string>()))
				DEBUG_ERROR("Failed to set string value");
			break;
		case json::value_t::boolean:
			if (!prop.set_value(object, value.get<bool>()))
				DEBUG_ERROR("Failed to set boolean value");
			break;
		case json::value_t::number_integer:
		{
			variant setvalue = value.get<int>();
			setvalue.convert(prop.get_type());
			prop.set_value(object, setvalue);
			break;
		}
		case json::value_t::number_unsigned:
		{
			variant setvalue = value.get<unsigned int>();
			setvalue.convert(prop.get_type());
			prop.set_value(object, setvalue);
			break;
		}
		case json::value_t::number_float:
		{
			variant setvalue = value.get<float>();
			setvalue.convert(prop.get_type());
			prop.set_value(object, setvalue);
			break;
		}
		case json::value_t::binary:
		case json::value_t::discarded:
		default:
			DEBUG_WARNING("Unuseable value");
			break;
	}
}

inline void JsonToObject(rttr::type ty, variant& object, const json& j) {
	// loop through all values in the json file
	for (auto it = j.begin(); it != j.end(); ++it) {
		string key = it.key();
		property prop = ty.get_property(key.c_str());
		if (!prop) DEBUG_WARNING("there was no property with name: " + it.key());
		else SetPropertyFromValue(object, prop, it.value().type(), it.value());
	}
}



#endif // !_OBJECTS_JSON_TO_OBJECT_LOADER_HPP