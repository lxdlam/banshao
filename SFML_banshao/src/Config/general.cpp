#include "general.h"
#include "../utils.h"
#include <fstream>

namespace game::Config
{
	config::config(std::string jsonPath)
	{
		this->jsonPath = jsonPath;
	}

	config::~config()
	{
		_json.clear();
	}

	int config::loadFile() noexcept
	{
		setDefaults();
		std::ifstream inFile(jsonPath);
		if (inFile.fail())
		{
			LOG(WARNING) << "Load config file failed: " << jsonPath << ", using default.";
			return -1;
		}

		json tmp;
		inFile >> tmp;
		return copyValues(tmp);
	}

	int config::saveFile() noexcept
	{
		std::ofstream outFile(jsonPath);
		if (outFile.fail())
		{
			LOG(ERROR) << "Save to config file failed: " << jsonPath;
			return -1;
		}

		outFile << _json.dump(4) << std::endl;
		return 0;
	}

	bool config::checkBool(json& j, const std::string& key) {
		if (j[key].is_boolean())
		{
			_json[key] = j[key];
			return false;
		}
		LOG(WARNING) << "Value Type(Bool) Check Error ( " << key << " : " << j[key] << " )";
		return true;
	}
	bool config::checkStr(json& j, const std::string& key) {
		if (j[key].is_string())
		{
			_json[key] = j[key];
			return false;
		}
		LOG(WARNING) << "Value Type(String) Check Error ( " << key << " : " << j[key] << " )";
		return true;
	}
	bool config::checkInt(json& j, const std::string& key) {
		if (j[key].is_number_integer())
		{
			_json[key] = j[key];
			return false;
		}
		LOG(WARNING) << "Value Type(Int) Check Error ( " << key << " : " << j[key] << " )";
		return true;
	}
	bool config::checkUnsigned(json& j, const std::string& key) {
		if (j[key].is_number_unsigned())
		{
			_json[key] = j[key];
			return false;
		}
		LOG(WARNING) << "Value Type(Unsigned) Check Error ( " << key << " : " << j[key] << " )";
		return true;
	}
}