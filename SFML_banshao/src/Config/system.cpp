#include "system.h"

using utils::log;

namespace game::Config
{
	system& system::getInstance()
	{
		static system _inst;
		return _inst;
	}

	int system::setDefaults()
	{
		//FIXME + time stamp
		setLogFile("log.txt");
		return 0;
	}

	int system::setLogFile(const std::string& path)
	{
		logFilePath = path;
		logFileStream.open(logFilePath, std::ios::out);
		if (logFileStream.is_open())
		{
			logStream = &logFileStream;
			log("[System] Logfile set to " + logFilePath, verbose_level::LOGS_Config);
			return 0;
		}
		else
		{
			log("[System] Failed to open log file " + logFilePath + ", using stdout instead", verbose_level::LOGS_Core);
			return 1;
		}
	}
	std::ostream* system::getLogStream()
	{
		return logStream;
	}

	std::string system::getTitle()
	{
		return title;
	}

	int system::setVerbose(verbose_level v)
	{
		verbose = v;
		log("[System] verbose level -> " + std::to_string(static_cast<int>(v)), verbose_level::LOGS_Config);
		return 0;
	}
	int system::getVerbose()
	{
		return verbose;
	}
}