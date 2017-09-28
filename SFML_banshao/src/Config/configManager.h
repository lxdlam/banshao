#pragma once
#include "audio.h"
#include "video.h"
#include "keyconfig.h"
#include "system.h"
#include "play.h"

namespace game
{
	class configManager
	{
	private:
		configManager();
		~configManager();
	public:
		configManager(configManager&) = delete;
		configManager& operator= (configManager&) = delete;
		static configManager& getInstance();

	public:
		Config::system system;
		Config::keyconfig key;
		Config::audio audio;
		Config::video video;
		Config::play play;

		void load();
		void save();
	};
}