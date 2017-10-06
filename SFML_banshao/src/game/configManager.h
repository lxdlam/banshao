#pragma once
#include "../Config/audio.h"
#include "../Config/video.h"
#include "../Config/keyconfig.h"
#include "../Config/system.h"
#include "../Config/play.h"

namespace game
{
	constexpr char configFile[] = "config/config.json";
	constexpr char keyconfigFile[] = "config/keyconfig.json";

	class configManager
	{
	private:
		configManager();
		~configManager();
	public:
		configManager(configManager&) = delete;
		configManager& operator= (configManager&) = delete;
		friend configManager& config();

	public:
		Config::system system;
		Config::keyconfig key;
		Config::audio audio;
		Config::video video;
		Config::play play;

		void load();
		void save();
		void saveKey();
	};

	static configManager& config()
	{
		static configManager _inst;
		return _inst;
	}
}