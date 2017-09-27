#include "play.h"

namespace game::Config
{
	play& play::getInstance()
	{
		static play _inst;
		return _inst;
	}

	int play::setDefaults()
	{
		setBaseSpeed(100);
		return 0;
	}

	int play::getBaseSpeed() const
	{
		return baseSpeed;
	}

	void play::setBaseSpeed(int b)
	{
		baseSpeed = b;
	}

}