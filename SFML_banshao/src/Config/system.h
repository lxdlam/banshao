#pragma once
#include "../utils.h"
#include <string>
#include <iostream>
#include <fstream>

namespace game::Config
{
	class system
	{
	private:
		system() = default;
		~system() = default;
	public:
		static system& getInstance();
		system(system const&) = delete;
		void operator=(system const&) = delete;
		int setDefaults();
		int loadConfig();
		
	private:
		const std::string title = "asdf";

		std::string logFilePath;
		std::fstream logFileStream;
		std::ostream *logStream = &std::cout;
		int verbose = LOGS_All;

	public:
		int setLogFile(const std::string& path);
		std::ostream* getLogStream();
		
		std::string getTitle();

		int setVerbose(verbose_level v);
		int getVerbose();

	};
}
