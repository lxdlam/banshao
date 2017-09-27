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

	public:
		std::string getTitle();
	};
}
