#include "general.h"
#include "../utils.h"
#include <fstream>

namespace game::Config
{
	config::config()
	{
		// placeholder, do nothing
	}

	config::config(std::string json) : _json(json) {}

	config::~config() {}

	bool config::checkBool(const json& j, const std::string& key) {
		if (j[key].is_boolean())
		{
			_json[key] = j[key];
			return false;
		}
		LOG(WARNING) << "Value Type(Bool) Check Error ( " << key << " : " << j[key] << " )";
		return true;
	}
	bool config::checkStr(const json& j, const std::string& key) {
		if (j[key].is_string())
		{
			_json[key] = j[key];
			return false;
		}
		LOG(WARNING) << "Value Type(String) Check Error ( " << key << " : " << j[key] << " )";
		return true;
	}
	bool config::checkInt(const json& j, const std::string& key) {
		if (j[key].is_number_integer())
		{
			_json[key] = j[key];
			return false;
		}
		LOG(WARNING) << "Value Type(Int) Check Error ( " << key << " : " << j[key] << " )";
		return true;
	}
	bool config::checkUnsigned(const json& j, const std::string& key) {
		if (j[key].is_number_unsigned())
		{
			_json[key] = j[key];
			return false;
		}
		LOG(WARNING) << "Value Type(Unsigned) Check Error ( " << key << " : " << j[key] << " )";
		return true;
	}

	void to_json(json& j, const config& o)
	{
		j = o._json;
	}

	void from_json(const json& j, config& o)
	{
		o.setDefaults();
		o.copyValues(j);
		o.checkValues();
	}
}