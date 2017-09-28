#pragma once

#include "../defs.h"

namespace game::Input
{
	class functional
	{
	public:
		functional() = delete;
		~functional() = delete;

		using keys = defs::functionalKeys;
		static unsigned long detect();
	};
}
