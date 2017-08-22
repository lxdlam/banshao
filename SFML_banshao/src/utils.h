#pragma once
#include <string>

enum verbose_level{
	LOGS_Minimum = 0,
	LOGS_Core,
	LOGS_Config,
	LOGS_All
};

namespace utils
{
	void log(std::string info, int verbose = LOGS_Minimum);
	void logSystemInfo();
}
