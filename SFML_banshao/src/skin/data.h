#pragma once
#include <array>
#include <vector>
#include "../defs.h"
#include <chrono>
using namespace std::chrono;

namespace game
{
	using namespace defs::skin;

	class dataClass
	{
	private:
		dataClass();
		~dataClass() = default;
	public:
		dataClass(dataClass&) = delete;
		dataClass& operator= (dataClass&) = delete;
		friend dataClass& data();

	protected:
		time_point<system_clock> skinStartTime;
		std::array<long long, static_cast<size_t>(timer::TIMER_COUNT)> _timer{};
		std::array<bool,     static_cast<size_t>(dst_option::DST_OPTION_COUNT)> _dstOption{};
		std::array<int,      static_cast<size_t>(num::NUM_COUNT)> _num{};
	public:
		void resetTimer();
		long long getTimeFromStart();
		long long getTimeFromTimer(timer);
		long long getTimeFromTimer(timer, long long time);
		void setTimer(timer, long long);
		unsigned getTimer(timer);
		void setDstOption(dst_option, bool);
		bool getDstOption(dst_option);
		void setNum(num, int);
		int getNum(num);

	protected:
		// init functions called in constructor.
		// Actual values are managed by the game.
		void resetDstOption();
		void resetNum();

	};
	
	dataClass& data();
}