#include "configManager.h"
#include <fstream>
#include "../utils.h"
#include "../external/json.hpp"

namespace game
{
	configManager::configManager()
	{
		system.setDefaults();
		video.setDefaults();
		audio.setDefaults();
		play.setDefaults();
		key.setDefaults();

		std::ifstream inFile(configFile);
		if (inFile.fail())
		{
			LOG(WARNING) << "Load config file failed: " << configFile << ", using default.";
			save();
		}
		else
		{
			// load config file
			using nlohmann::json;
			json conf;
			inFile >> conf;

			// initialize sub settings
			system = conf["System"];
			video = conf["Video"];
			audio = conf["Audio"];
			play = conf["Play"];
		}

		// key configs are stored in another file
		key.loadFile(keyconfigFile);
	}

	configManager::~configManager()
	{
		save();
		saveKey();
	}

	void configManager::save()
	{
		std::ofstream outFile(configFile);
		if (outFile.fail())
		{
			LOG(ERROR) << "Save to config file failed: " << configFile;
			return;
		}
		using nlohmann::json;
		json conf;
		conf["System"] = system;
		conf["Video"] = video;
		conf["Audio"] = audio;
		conf["Play"] = play;
		outFile << conf.dump(4) << std::endl;
	}

	void configManager::saveKey()
	{
		key.saveFile(keyconfigFile);
	}
}
