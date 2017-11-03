#include "configBase.h"
#include "../utils.h"
#include <fstream>

namespace game {
	namespace config

	{
		configBase::configBase()
		{
			// placeholder, do nothing
		}

		configBase::configBase(std::string json) : _json(json) {}

		configBase::~configBase() {}

		bool configBase::checkBool(const json& j, const std::string& key) {
			if (j[key].is_boolean())
			{
				_json[key] = j[key];
				return false;
			}
			LOG(WARNING) << "Value Type(Bool) Check Error ( " << key << " : " << j[key] << " )";
			return true;
		}
		bool configBase::checkStr(const json& j, const std::string& key) {
			if (j[key].is_string())
			{
				_json[key] = j[key];
				return false;
			}
			LOG(WARNING) << "Value Type(String) Check Error ( " << key << " : " << j[key] << " )";
			return true;
		}
		bool configBase::checkInt(const json& j, const std::string& key) {
			if (j[key].is_number_integer())
			{
				_json[key] = j[key];
				return false;
			}
			LOG(WARNING) << "Value Type(Int) Check Error ( " << key << " : " << j[key] << " )";
			return true;
		}
		bool configBase::checkUnsigned(const json& j, const std::string& key) {
			if (j[key].is_number_unsigned())
			{
				_json[key] = j[key];
				return false;
			}
			LOG(WARNING) << "Value Type(Unsigned) Check Error ( " << key << " : " << j[key] << " )";
			return true;
		}

		void to_json(json& j, const configBase& o)
		{
			j = o._json;
		}

		void from_json(const json& j, configBase& o)
		{
			o.setDefaults();
			o.copyValues(j);
			o.checkValues();
		}
	}
}