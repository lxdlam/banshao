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

namespace utils
{
	void log(std::string info, int verbose)
	{
		if (verbose > game::Config::system::getInstance().getVerbose()) return;

		auto sysclock = std::chrono::system_clock::now();
		auto ttclock = std::chrono::system_clock::to_time_t(sysclock);
		char timestr[100];
		std::strftime(timestr, sizeof(timestr), "%c", std::localtime(&ttclock));

		auto outStream = game::Config::system::getInstance().getLogStream();
		(*outStream) << '[' << timestr << ']' << ' ';
		(*outStream) << info << std::endl;
	}

	void logSystemInfo()
	{
#if defined(SFML_SYSTEM_WINDOWS)
		// Seems it's not accurate.
		// Returns Windows 8 on my win10 PC.
		if (IsWindows10OrGreater())				log("OS: Windows 10");
		else if (IsWindows8Point1OrGreater())	log("OS: Windows 8.1");
		else if (IsWindows8OrGreater())			log("OS: Windows 8");
		else if (IsWindows7SP1OrGreater())		log("OS: Windows 7 SP1");
		else if (IsWindows7OrGreater())			log("OS: Windows 7");
		else									log("OS: Windows");
#elif defined(SFML_SYSTEM_LINUX)
		// FIXME Not like this
		// maybe system("asdf")
		log("OS: " + GetStdoutFromCommand("lsb_release -ds") + "(" GetStdoutFromCommand("uname -mrs") + ")");
#elif defined(SFML_SYSTEM_MACOS)
		log("OS: Mac OS X " + GetStdoutFromCommand("sw_vers -productVersion"));
#else
		log("OS: Unknown");
#endif
		// TODO more info
	}
}