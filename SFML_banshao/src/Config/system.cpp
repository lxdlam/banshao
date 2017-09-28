#include "system.h"
#include "../defs.h"

namespace game::Config
{
	using namespace defs;
	void system::setDefaults() noexcept
	{	
		set<unsigned>(sys_basespd, 100);
	}

	int system::copyValues(json& j) noexcept 
	{
		return checkUnsigned(j, sys_basespd) ? 1 : 0;
	}
}