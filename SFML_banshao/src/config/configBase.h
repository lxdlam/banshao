#pragma once
#include <string>
#include "../external/json.hpp"

namespace game {
	namespace config

	{
		using json = nlohmann::json;

		class configBase
		{
		protected:
			json _json;

		public:
			configBase();
			configBase(std::string json);
			~configBase();

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
			virtual int copyValues(const json& j) noexcept = 0;
			virtual int checkValues() noexcept = 0;
			friend void to_json(json& j, const configBase& o);
			friend void from_json(const json& j, configBase& o);

			bool checkBool(const json& j, const std::string& key);
			bool checkStr(const json& j, const std::string& key);
			bool checkInt(const json& j, const std::string& key);
			bool checkUnsigned(const json& j, const std::string& key);
		};

		void to_json(json& j, const configBase& o);
		void from_json(const json& j, configBase& o);
	}
}