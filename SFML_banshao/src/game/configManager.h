#pragma once
#include "../config/audio.h"
#include "../config/video.h"
#include "../config/keyconfig.h"
#include "../config/system.h"
#include "../config/play.h"
#include "../defs.h"

namespace game
{
	using namespace defs::file;

	class configManager
	{
	private:
		configManager();
		~configManager();
	public:
		configManager(configManager&) = delete;
		configManager& operator= (configManager&) = delete;
		friend configManager& conf();

	public:
		config::system system;
		config::keyconfig key;
		config::audio audio;
		config::video video;
		config::play play;

		void load();
		void loadKey();
		void save();
		void saveKey();
	};

	static configManager& conf()
	{
		static configManager _inst;
		return _inst;
	}
}