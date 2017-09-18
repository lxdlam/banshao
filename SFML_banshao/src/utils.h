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

	constexpr int base36(char c)
	{
		return (c > '9') ? (10 + c - 'A') : (c - '0');
	}

	constexpr int base36(char first, char second)
	{
		return 36 * base36(first) + base36(second);
	}

	constexpr int base36(const char* c)
	{
		return base36(c[0], c[1]);
	}

	constexpr int base16(char c)
	{
		return (c > '9') ? (10 + c - 'A') : (c - '0');
	}

	constexpr int base16(char first, char second)
	{
		return 16 * base16(first) + base16(second);
	}

	constexpr int base16(const char* c)
	{
		return base16(c[0], c[1]);
	}
}
