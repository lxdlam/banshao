#pragma once
#include <string>

namespace game::Config
{

	//const std::string configFile = "config.json";

	int init();
	int initWithConfig();
	int saveConfig();

	int setDefaults();
}