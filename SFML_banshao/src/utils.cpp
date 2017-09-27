#include "utils.h"
#include "Config/system.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>

#include <SFML/Config.hpp>
#if defined(SFML_SYSTEM_WINDOWS)
#include <windows.h>
#include <VersionHelpers.h>
#pragma comment(lib, "user32.lib")
#endif

INITIALIZE_EASYLOGGINGPP

namespace utils
{
	void initLogging()
	{
		el::Configurations conf;
		conf.setToDefault();
		conf.setGlobally(el::ConfigurationType::Format, "%datetime{%Y-%M-%d %H:%m:%s} %levshort: %msg");
		conf.setGlobally(el::ConfigurationType::Filename, "log.txt");
		el::Loggers::reconfigureLogger("default", conf);
	}
}