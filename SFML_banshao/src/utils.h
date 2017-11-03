#pragma once
#include <filesystem>
#include <vector>
#include <utility>
#include <iostream>
#include <fstream>
#include "types.hpp"

enum severity_level
{
	DEBUG,
	INFO,
	WARNING,
	ERROR,
	FATAL
};

std::ostream& LOG(severity_level);
namespace utils
{
	void initLogging();
	static std::ofstream logNullSink;
	static std::ostream* logSink = &std::cout;
	std::ostream& getLogSink(severity_level);

	std::vector<fs::path> findFiles(fs::path path);
	std::pair<unsigned, bool> stoub(const std::string&);

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
