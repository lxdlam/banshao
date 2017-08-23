#include "utils.h"
#include "Config/general.h"
#include "gameInstance.h"


using utils::log;

namespace game
{
	gameInstance& gameInstance::getInstance()
	{
		static gameInstance _inst;
		return _inst;
	}

	gameInstance::gameInstance()
	{
		// TODO init with file
		Config::init();
		utils::logSystemInfo();
		window.setWindowMode();
	}

	gameInstance::~gameInstance()
	{
		log("-------------------------------------------------------------");

		Config::saveConfig();
		Config::dispose();
	}

	int gameInstance::run()
	{
		window.run();
		return 0;
	}

	int gameInstance::getWindowFPS()
	{
		return window.getFPS();
	}
}