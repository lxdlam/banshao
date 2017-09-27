#include "system.h"

namespace game::Config
{
	system& system::getInstance()
	{
		static system _inst;
		return _inst;
	}

	int system::setDefaults()
	{
		return 0;
	}

	std::string system::getTitle()
	{
		return title;
	}

}