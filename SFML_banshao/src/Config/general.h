#pragma once
#include <string>
#include "../external/json.hpp"

namespace game::Config
{
	class config
	{
	public:
		using json = nlohmann::json;

	protected:
		json _json;
		std::string jsonPath;

	public:
		config() = default;
		config(std::string jsonPath);
		~config();

		virtual int loadFile() noexcept;
		int saveFile() noexcept;
		virtual void setDefaults() noexcept = 0;

		template<class Ty_v>
		Ty_v get(const std::string key) { return _json[key]; }
		template<class Ty_v>
		void set(const std::string key, const Ty_v value) noexcept { _json[key] = value; }

	protected:
		// Remove unnecessary objects and check if values in json object is correct.
		// If incorrect value was found, set to default value.
		// Automatically called in loadFile() function.
		// Returns incorrect value count.
		virtual int copyValues(json& j) noexcept = 0;

		bool checkBool(json& j, const std::string& key);
		bool checkStr(json& j, const std::string& key);
		bool checkInt(json& j, const std::string& key);
		bool checkUnsigned(json& j, const std::string& key);
	};
}