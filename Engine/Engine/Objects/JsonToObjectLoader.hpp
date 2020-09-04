#ifndef _OBJECTS_JSON_TO_OBJECT_LOADER_HPP
#define _OBJECTS_JSON_TO_OBJECT_LOADER_HPP
#include "../General/Serialization.hpp"
#include "../Core/Debugger.hpp"

void JsonToObject(rttr::type ty, variant& object, const json& j);
//void SetPropertyFromValue(variant& object, property& prop, const json::value_t& jtype, const json& value);
void SetVariantFromValue(variant& vari, const type& propty, const json& jvalue);

inline void JsonToObject(rttr::type ty, variant& object, const json& j) {
	// loop through all values in the json file
	for (auto it = j.begin(); it != j.end(); ++it) {
		string key = it.key();
		property prop = ty.get_property(key.c_str());
		if (!prop) DEBUG_WARNING("there was no property with name: " + it.key());
		else {
			variant vari = prop.get_value(object);
			SetVariantFromValue(vari, prop.get_type(), it.value());
			prop.set_value(object, vari);
		}
		//SetPropertyFromValue(object, prop, it.value().type(), it.value());
	}
}

//inline void SetPropertyFromValue(variant& object, property& prop, const json::value_t& jtype, const json& value) { }

inline void SetVariantFromValue(variant& vari, const type& propty, const json& jvalue) {
	switch (jvalue.type()) {
		case json::value_t::null:
			break;
		case json::value_t::object:
			// recurse
			JsonToObject(vari.get_type(), vari, jvalue);
			break;
		case json::value_t::array:
		{
			if (!vari.is_array()) {
				DEBUG_ERROR("Cannot set property as its not an array");
				break;
			}
			variant_array_view varr = vari.create_array_view();
			
			// if its dynamic then make sure you set the size
			if (varr.is_dynamic())
				varr.set_size(jvalue.size());

			// get the type inside the array
			type arrsubty = varr.get_value(0).get_type();
			
			// fill the array
			for (size_t i = 0; i < jvalue.size() && i < varr.get_size(); ++i) {
				variant setvalue;
				SetVariantFromValue(setvalue, arrsubty, jvalue[i]);
				varr.set_value(i, setvalue);
			}
			
			break;
		}
		case json::value_t::string:
			vari = jvalue.get<string>();
			break;
		case json::value_t::boolean:
			vari = jvalue.get<bool>();
		case json::value_t::number_integer:
			vari = jvalue.get<int>();
			vari.convert(propty);
			break;
		case json::value_t::number_unsigned:
			vari = jvalue.get<unsigned int>();
			vari.convert(propty);
			break;
		case json::value_t::number_float:
			vari = jvalue.get<float>();
			vari.convert(propty);
			break;
		case json::value_t::binary:
		case json::value_t::discarded:
		default: DEBUG_WARNING("Unuseable value"); break;
	}
}




#endif // !_OBJECTS_JSON_TO_OBJECT_LOADER_HPP