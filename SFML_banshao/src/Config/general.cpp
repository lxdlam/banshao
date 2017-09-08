#include "general.h"
#include "system.h"
#include "video.h"
#include "audio.h"
#include "key.h"
//#include "../lib/json.hpp"
#include <fstream>

namespace game::Config
{
	int init()
	{
		setDefaults();
		return 0;
	}

	int initWithConfig()
	{
		setDefaults();

		// TODO json
		//using json = nlohmann::json;
		//std::ifstream i(configFile);
		//json file;
		//i >> file;


		return 0;
	}

	int saveConfig()
	{
		// TODO save to config file
		return 0;
	}

	int setDefaults()
	{
		system::getInstance().setDefaults();
		video::getInstance().setDefaults();
		audio::getInstance().setDefaults();
		key::getInstance().setDefaults();
		return 0;
	}
}