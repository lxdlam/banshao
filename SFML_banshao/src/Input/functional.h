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

		static constexpr unsigned long mask(keys k)
		{
			return 1 << k;
		}

		static unsigned long detect();
	};
}
