#pragma once

#include <bitset>
#include "../defs.h"

namespace game {
	namespace Input

	{
		using namespace defs::key;

		class functional
		{
		public:
			functional() = delete;
			~functional() = delete;

			using keys = functionalKeys;
			static std::bitset<keys::FUNC_KEY_COUNT> detect();
		};
	}
}