#include "data.h"

namespace game
{
	dataClass::dataClass()
	{
		resetTimer();
		resetDstOption();
		resetNum();
	}

	dataClass& data()
	{
		static dataClass _inst;
		return _inst;
	}

	unsigned dataClass::getTimeFromStart()
	{
		return duration_cast<milliseconds>(system_clock::now() - skinStartTime).count() + 1;
	}

	int dataClass::getTimeFromTimer(timer timer)
	{
		int rt = getTimeFromStart() - getTimer(timer);
		if (rt < 0)
			return -1;
		else
			return rt;
	}

	void dataClass::setTimer(timer dst, unsigned src)
	{
		_timer[static_cast<size_t>(dst)] = src;
	}

	unsigned dataClass::getTimer(timer dst)
	{
		return _timer[static_cast<size_t>(dst)];
	}

	void dataClass::setDstOption(dst_option dst, bool src)
	{
		_dstOption[static_cast<size_t>(dst)] = src;
	}

	bool dataClass::getDstOption(dst_option dst)
	{
		return _dstOption[static_cast<size_t>(dst)];
	}

	void dataClass::setNum(num dst, int src)
	{
		_num[static_cast<size_t>(dst)] = src;
	}

	int dataClass::getNum(num dst)
	{
		return _num[static_cast<size_t>(dst)];
	}

	void dataClass::resetTimer()
	{
		_timer.fill(0);
		skinStartTime = system_clock::now();
		setTimer(timer::SCENE_START, getTimeFromStart());
	}

	void dataClass::resetDstOption()
	{
		_dstOption.fill(false);
		setDstOption(dst_option::TRUE, true);
	}

	void dataClass::resetNum()
	{
		_num.fill(0);
	}
}