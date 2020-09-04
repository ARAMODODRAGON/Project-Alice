#ifndef _OBJECTS_JSON_TO_OBJECT_LOADER_HPP
#define _OBJECTS_JSON_TO_OBJECT_LOADER_HPP
#include "../General/Serialization.hpp"
#include "../Core/Debugger.hpp"

void JsonToObject(rttr::type ty, variant& object, const json& j);
void SetVariantFromValue(variant& vari, const type& propty, const json::value_t& jtype, const json& jvalue);

inline void JsonToObject(rttr::type ty, variant& object, const json& j) {
	// loop through all values in the json file
	for (auto it = j.begin(); it != j.end(); ++it) {
		string key = it.key();
		property prop = ty.get_property(key.c_str());
		if (!prop) DEBUG_WARNING("there was no property with name: " + it.key());
		else {
			// set the property
			variant vari = prop.get_value(object);
			SetVariantFromValue(vari, prop.get_type(), j.type(), j);
			prop.set_value(object, vari);
		}
		///SetPropertyFromValue(object, prop, it.value().type(), it.value());
	}
}

inline void SetVariantFromValue(variant& vari, const type& propty, const json::value_t& jtype, const json& jvalue) {
	switch (jtype) {
		case json::value_t::null:
			break;
		case json::value_t::object:
		{
			//variant subobject = prop.get_value(object);

			// recurse
			JsonToObject(vari.get_type(), vari, jvalue);

			//if (!prop.set_value(object, subobject))
			//	DEBUG_ERROR("Failed to set value");
			break;
		}
		case json::value_t::array:
		{
			//DEBUG_WARNING("Cannot initialize arrays");
			//variant_array_view varr = vari.create_array_view();
			//if (!varr.is_valid()) {
			//	DEBUG_ERROR("Cannot set property using array value");
			//	break;
			//}
			//
			//// if its dynamic then make sure you set the size
			//if (varr.is_dynamic())
			//	varr.set_size(jvalue.size());
			//
			//// fill the array
			//for (size_t i = 0; i < jvalue.size(); ++i) {
			//	
			//}

			break;
		}
		case json::value_t::string:
			//if (!prop.set_value(object, value.get<string>()))
			//	DEBUG_ERROR("Failed to set string value");
			vari = jvalue.get<string>();
			break;
		case json::value_t::boolean:
			//if (!prop.set_value(object, value.get<bool>()))
			//	DEBUG_ERROR("Failed to set boolean value");
			vari = jvalue.get<bool>();
			break;
		case json::value_t::number_integer:
		/*{
			variant setvalue = value.get<int>();
			setvalue.convert(prop.get_type());
			prop.set_value(object, setvalue);
		}*/
			vari = jvalue.get<int>();
			vari.convert(propty);
			break;
		case json::value_t::number_unsigned:
		/*{
			variant setvalue = value.get<unsigned int>();
			setvalue.convert(prop.get_type());
			prop.set_value(object, setvalue);
		}*/
			vari = jvalue.get<unsigned int>();
			vari.convert(propty);
			break;
		case json::value_t::number_float:
		/*{
			variant setvalue = value.get<float>();
			setvalue.convert(prop.get_type());
			prop.set_value(object, setvalue);
		}*/
			vari = jvalue.get<float>();
			vari.convert(propty);
			break;
		case json::value_t::binary:
		case json::value_t::discarded:
		default:
			DEBUG_WARNING("Unuseable value");
			break;
	}
}

#endif // !_OBJECTS_JSON_TO_OBJECT_LOADER_HPP