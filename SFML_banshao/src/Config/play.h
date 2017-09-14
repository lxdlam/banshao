#pragma once
#include "../utils.h"
#include <string>
#include <iostream>
#include <fstream>

namespace game::Config
{
	class play
	{
	private:
		play() = default;
		~play() = default;
	public:
		static play& getInstance();
		play(play const&) = delete;
		void operator=(play const&) = delete;
		int setDefaults();
		int loadConfig();
		
	private:
		int baseSpeed = 100;
	public:
		int getBaseSpeed() const;
		void setBaseSpeed(int);

	};
}
