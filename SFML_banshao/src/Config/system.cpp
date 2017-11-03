#include "system.h"
#include "../defs.h"

namespace game {
	namespace config

	{
		using namespace defs::config;

		void system::setDefaults() noexcept
		{
			set<unsigned>(sys_basespd, 100);
		}

		int system::copyValues(const json& j) noexcept
		{
			return checkUnsigned(j, sys_basespd) ? 1 : 0;
		}

		int system::checkValues() noexcept
		{
			int c = 0;

			// base speed
			if (get<unsigned>(sys_basespd) == 0)
			{
				c++;
				set<unsigned>(sys_basespd, 100);
			}

			return c;
		}
	}
}