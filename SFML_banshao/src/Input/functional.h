#pragma once

#include "../defs.h"

namespace game::Input
{
	using namespace defs::key;

	class functional
	{
	public:
		functional() = delete;
		~functional() = delete;

		using keys = functionalKeys;
		static unsigned long detect();
	};
}
